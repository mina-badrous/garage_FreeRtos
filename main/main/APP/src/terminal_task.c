/*
 * terminal_task.c
 *
 * Created: 02/06/2018 16:18:52
 *  Author: BADROUS
 */ 
#include "terminal_task.h"
//------------------------------------------------------------------------
static u8 u8Get_at_char(void);
static u8 u8Get_input_buffer(void);
//-------------------------------
static u8 get_user_input(void);
//-------------------------------
static u8 u8Get_user_password(void);
//----------------------------------------------------------------------
static xQueueHandle g_terminal_task_q;
//----------------------------------------------------------------------------
static	u8 g_command_array[MAX_TERMINAL_BUFFER_DATA];
static	u8 g_command_index=0;	
//---------------------------------------------------------------------------
static void vMenu_options_print(void);
static void vMenu_users_options_print(void);
//----------------------------------------------------------------------
static void invalid_password_print(void);
//---------------------------------------------------------------------
static void menu_users_choose(void);
static void main_menu_choose(void);
//-------------------------------------------------------------------
static void vTerminal_factory_state(void);
static void vTerminal_logged_off_state(void);
static void vTerminal_check_admin_pass_state(void);
static void vTerminal_users_menu_state(void);
static void vTerminal_locked_state(void);
//-------------------------------------------------------------------
typedef void (*PTR2FUNC)(void);
PTR2FUNC menu_func_ptr[MAX_MENU_OPTIONS]={unlock_system,open_garage,close_garage,set_time,NULL,NULL};	
PTR2FUNC user_menu_ptr[MAX_USERS_MENU_OPTIONS]={list_users,add_users,edit_users,delete_users,factory_reset,NULL};	
//-----------------------------------------------------------------------
terminal_task_state_t task_state=TERMINAL_TASK_LOGGED_OFF_STATE;
u8 admin_invalid_count=0;	
user_t current_admin;
u8 current_id;
//----------------------------------------------------------------	
void vTerminal_task(void *pvParameters)
{	
	while(1)
	{
		switch(task_state)
		{
			case TERMINAL_TASK_FACTORY_STATE :
											vTerminal_factory_state();	
											break;
			case TERMINAL_TASK_LOGGED_OFF_STATE :
											vTerminal_logged_off_state();	
											break;	
			case TERMINAL_TASK_CHECK_ADMIN_PASS_STATE :
											vTerminal_check_admin_pass_state();
											break;
			case TERMINAL_TASK_MENU_STATE :
											vMenu_options_print();
											main_menu_choose();
											break;
			case TERMINAL_TASK_USERS_MENU_STATE :
											vTerminal_users_menu_state();
											break;
			case TERMINAL_TASK_LOCKED_STATE :
											vTerminal_locked_state();
											break;
			default:break;
		}
	}
}

//-------------------------------------------------------
void vTerminal_task_q_set(xQueueHandle key)
{
	g_terminal_task_q=key;
}
//---------------------------------------------------------
xQueueHandle xTerminal_task_q_get(void)
{
	return g_terminal_task_q;
}
//----------------------------------------------ISR call back FUNCTION
void rx_complete_isr_func(void)
{
	u8 data=UDR;
	xQueueSendToBackFromISR(g_terminal_task_q,(void*)&data,0);
}
//----------------------------------------------------------------------get user input data
u8 u8Get_input_buffer(void)
{
	portBASE_TYPE que_state;
	u8 data;
	//reset default values
	for(g_command_index=0;g_command_index<MAX_TERMINAL_BUFFER_DATA;g_command_index++)
	{
		g_command_array[g_command_index]=0;
	}
	g_command_index=0;
	while(1)
		{
			que_state= xQueueReceive(g_terminal_task_q,&data,10000/ portTICK_PERIOD_MS);
			if(que_state ==pdPASS)
			{
				switch(data)
				{
					case ENTER_KEY :
										//signal shell parser to start parsing command
										return ENTER_KEY ;
					case BACKSPACE_KEY :
										//delete last char
										g_command_index--;
										if(g_command_index==255)
										{
											g_command_index=0;
										}
										g_command_array[g_command_index]=0;
										break;
					case AT_KEY :
								//return @ key
								return AT_KEY;
					default:
								//fill the command array
								g_command_array[g_command_index]=data;
								g_command_index++;
								if(g_command_index==MAX_TERMINAL_BUFFER_DATA)
								{
									g_command_index--;
								}
								break;
				}
			}
			else
			{
				//time out finish
				return TERMINAL_TIME_OUT;
			}
			
		}
		
}
//-----------------------------------------------------------------------------------
static void vMenu_options_print(void)
{
	xSemaphoreTake(uart_mutex,portMAX_DELAY);
	
	vUartTransmit(ENTER_KEY);
	vUartTransmitString_program(PSTR("ENTER 1 TO UNLOCK THE SYSTEM"));
	vUartTransmit(ENTER_KEY);
	vUartTransmitString_program(PSTR("ENTER 2 TO OPEN GARAGE"));
	vUartTransmit(ENTER_KEY);	
	vUartTransmitString_program(PSTR("ENTER 3 TO CLOSE GARAGE"));
	vUartTransmit(ENTER_KEY);
	vUartTransmitString_program(PSTR("ENTER 4 TO SET THE TIME"));
	vUartTransmit(ENTER_KEY);
	vUartTransmitString_program(PSTR("ENTER 5 TO ADD/REMOVE USERS"));
	vUartTransmit(ENTER_KEY);
	vUartTransmitString_program(PSTR("ENTER 6 TO LOGOUT"));
	vUartTransmit(ENTER_KEY);
	vUartTransmit(ENTER_KEY);
		
	xSemaphoreGive(uart_mutex);				
}
//-------------------------------------------------------------------------------
static void vMenu_users_options_print(void)
{
	xSemaphoreTake(uart_mutex,portMAX_DELAY);
	
	vUartTransmit(ENTER_KEY);
	vUartTransmitString_program(PSTR("ENTER 1 TO LIST ALL USERS"));
	vUartTransmit(ENTER_KEY);
	vUartTransmitString_program(PSTR("ENTER 2 TO ADD NEW USER"));
	vUartTransmit(ENTER_KEY);	
	vUartTransmitString_program(PSTR("ENTER 3 TO EDIT USER"));
	vUartTransmit(ENTER_KEY);
	vUartTransmitString_program(PSTR("ENTER 4 TO DELETE USER"));
	vUartTransmit(ENTER_KEY);
	vUartTransmitString_program(PSTR("ENTER 5 TO RESET FACTORY DEFAULT"));
	vUartTransmit(ENTER_KEY);
	vUartTransmitString_program(PSTR("ENTER 6 TO BACK TO MENU"));
	vUartTransmit(ENTER_KEY);
	vUartTransmit(ENTER_KEY);
	
	xSemaphoreGive(uart_mutex);
}
//------------------------------------------------------------------------------
u8 get_user_input(void)
{
	u8 input=0;
	//check g_buffer index if not 1 invalid input
	if((g_command_index >1) || (g_command_index ==0))
	{
		return INVALID_INPUT;
	}
	else 
	{
		//check valid user input
		input=g_command_array[0] - '0' - 1;
		if((input >=0) && (input <MAX_MENU_OPTIONS))
		{
			return input;
		}
		else
		{
			return INVALID_INPUT;
		}
	}
}
//-----------------------------------------------------------------------------------
u8 u8Get_at_char(void)
{
	u8 data;
	while(1)
	{
		xQueueReceive(g_terminal_task_q,&data,portMAX_DELAY);
		if(data ==AT_KEY)
		{
			return AT_KEY;
		}
		else
		{
			return INVALID_INPUT;
		}
	}
}
//-----------------------------------------------------------------------------------
u8 get_u8(u8* data)
{
	u8 first,second,third,result;
	result=u8Get_input_buffer();
	if(result==ENTER_KEY)
	{
		//if 1 digit
		if(g_command_index == 1)
		{
			first=g_command_array[0] - '0';
			*data=first;
			return TERMINAL_RETURN_U8;
		}
		//if 2 digit
		else if(g_command_index == 2)
		{
			second=g_command_array[0] - '0';
			first=g_command_array[1] - '0';
			
			*data=first+(second*10);
			return TERMINAL_RETURN_U8;
		}
		else if(g_command_index == 3)
		{
			third=g_command_array[0] - '0';
			second=g_command_array[1] - '0';
			first=g_command_array[2] - '0';
			if(third >= 2 )
			{
				if(second >=5)
				{
					if(first >5)
					{
						//error
						return TERMINAL_TIME_OUT;
					}
				}
			}
			*data=first+(second*10)+(third*100);
			return TERMINAL_RETURN_U8;
		}
		else
		{
			return TERMINAL_TIME_OUT;
		}
	}
	else
	{
		return TERMINAL_TIME_OUT;
	}
	
}
//-----------------------------------------------------------------------
u8 get_user_name(u8* data)
{
	u8 first,result;
	result=u8Get_input_buffer();
	if(result==ENTER_KEY)
	{
		//if > length
		if(g_command_index > MAX_USER_NAME+1)
		{
			
			return TERMINAL_RETURN_U8;
		}
		else
		{
			for(first=0;first<g_command_index;first++)
			{
				data[first]=g_command_array[first];
			}
			data[first]='\0';
			return VALID_NAME;
		}
		
	}
	else
	{
		return TERMINAL_TIME_OUT;
	}
	return TERMINAL_TIME_OUT;
}
//-------------------------------------------------
void invalid_password_print(void)
{
	xSemaphoreTake(uart_mutex,portMAX_DELAY);
	vUartTransmit(ENTER_KEY);
	vUartTransmitString_program(PSTR("Wrong entry "));
	vUartTransmit(admin_invalid_count+48+1);
	vUartTransmit(ENTER_KEY);
	xSemaphoreGive(uart_mutex);
	admin_invalid_count++;
	if(admin_invalid_count ==3)
	{
		//lock for root user
		task_state=TERMINAL_TASK_LOCKED_STATE;
		return;
	}
	else
	{
		//else change state to logged of state and increment the failed count
		task_state=TERMINAL_TASK_CHECK_ADMIN_PASS_STATE;
		return;
	}
}
//----------------------------------------------------------------
void menu_users_choose(void)
{
	u8 returned_key;
	vUartTransmitString_program(PSTR(">>>"));
	returned_key=u8Get_input_buffer();
	if(returned_key ==ENTER_KEY)
	{
		//check input
		returned_key=get_user_input();
		if (returned_key !=INVALID_INPUT)
		{
			if(returned_key ==BACK_TO_MAIN_MENU)
			{
				task_state=TERMINAL_TASK_MENU_STATE;
			}
			else
			{
				user_menu_ptr[returned_key]();
			}
		}
	}
	else if(returned_key==TERMINAL_TIME_OUT)
	{
		task_state=TERMINAL_TASK_LOGGED_OFF_STATE;
	}
}
//--------------------------------------------------
void main_menu_choose(void)
{
	u8 returned_key;
	vUartTransmitString_program(PSTR(">>>"));
	returned_key=u8Get_input_buffer();
	if(returned_key ==ENTER_KEY)
	{
		//check input
		returned_key=get_user_input();
		if (returned_key !=INVALID_INPUT)
		{
			if(returned_key ==USERS_MENU)
			{
				task_state=TERMINAL_TASK_USERS_MENU_STATE;
			}
			else if(returned_key==LOG_OUT)
			{
				task_state=TERMINAL_TASK_LOGGED_OFF_STATE;
			}
			else
			{
				menu_func_ptr[returned_key]();
			}
			//call functions pointer of index
		}
	
	}
	else if(returned_key==TERMINAL_TIME_OUT)
	{
		task_state=TERMINAL_TASK_LOGGED_OFF_STATE;
	}
}
//------------------------------------------------------
u8 u8Get_user_password(void)
{
	u8 returned_key;
	u8 error;
	user_t temp;
	u8 temp_id;
	returned_key=u8Get_input_buffer();
	if(returned_key ==ENTER_KEY)
	{
		//check password
		if(g_command_index !=4)
		{
			return PASSWORD_NOT_FOUND;
		}
		//check password
		xSemaphoreTake(i2c_mutex,portMAX_DELAY);
		error=u8EXTERNAL_EEPROM_search_by_password(g_command_array,temp.name,&temp_id,&(temp.type));
		xSemaphoreGive(i2c_mutex);
		if(error ==PASSWORD_FOUND)
		{
			current_id=temp_id;
			current_admin=temp;
			admin_invalid_count=0;
			return PASSWORD_FOUND;
		}			
	}
	else if(returned_key==TERMINAL_TIME_OUT)
	{
		task_state=TERMINAL_TASK_LOGGED_OFF_STATE;
		return TERMINAL_TIME_OUT;
	}
	else
	{
		return PASSWORD_NOT_FOUND;
	}
	return PASSWORD_NOT_FOUND;
}
//----------------------------------------------------------
u8 u8Get_user_password_arr(u8 * password)
{
	u8 returned_key;
	u8 index;
	returned_key=u8Get_input_buffer();
	if(returned_key ==ENTER_KEY)
	{
		//check password
		if(g_command_index !=4)
		{
			return PASSWORD_NOT_VALID;
		}
		else
		{
			for(index=0;index<MAX_USER_PASSWORD;index++)
			{
				password[index]=g_command_array[index];
			}
			return PASSWORD_VALID;
		}
	
	}
	else if(returned_key==TERMINAL_TIME_OUT)
	{
		task_state=TERMINAL_TASK_LOGGED_OFF_STATE;
		return PASSWORD_NOT_VALID;
	}
	else
	{
		return PASSWORD_NOT_VALID;
	}
	return PASSWORD_NOT_VALID;
}
//---------------------------------------------------------
u8 u8Get_terminal_task_state(void)
{
	return task_state;
}
void vSet_terminal_task_state(u8 state)
{
	task_state=state;
}
//-----------------------------------------------------------
static void vTerminal_factory_state(void)
{
	u8 returned_key;
	xSemaphoreTake(i2c_mutex,portMAX_DELAY);
	vEXTERNAL_EEPROM_reset();
	xSemaphoreGive(i2c_mutex);
	
	
	//enter password
	xSemaphoreTake(uart_mutex,portMAX_DELAY);
	
	vUartTransmit(ENTER_KEY);
	vUartTransmitString_program(PSTR("ENTER ROOT PASSWORD : "));
	xSemaphoreGive(uart_mutex);
	returned_key=u8Get_user_password();
	if(returned_key==PASSWORD_FOUND)
	{
		xSemaphoreTake(uart_mutex,portMAX_DELAY);
		vUartTransmit(ENTER_KEY);
		vUartTransmitString_program(PSTR("LOGGING IN ....... "));
		vUartTransmit(ENTER_KEY);
		vUartTransmit(ENTER_KEY);
		//change root password
		vUartTransmitString_program(PSTR("CHANGE ROOT PASSWORD : "));
		xSemaphoreGive(uart_mutex);
		returned_key=u8Get_user_password_arr(current_admin.password);
		if(returned_key==PASSWORD_VALID)
		{
			//set password
			current_admin.name[0]='R';
			current_admin.name[1]='O';
			current_admin.name[2]='O';
			current_admin.name[3]='T';
			current_admin.type=ROOT_USER;
			//edit user
			xSemaphoreTake(i2c_mutex,portMAX_DELAY);
			vEXTERNAL_EEPROM_edit_user(&current_admin,0);
			xSemaphoreGive(i2c_mutex);
			//set time
			xSemaphoreTake(uart_mutex,portMAX_DELAY);
			vUartTransmit(ENTER_KEY);
			vUartTransmitString_program(PSTR("PLEASE SET THE TIME ....... "));
			vUartTransmit(ENTER_KEY);
			vUartTransmit(ENTER_KEY);
			xSemaphoreGive(uart_mutex);
			set_time();
			task_state=TERMINAL_TASK_LOGGED_OFF_STATE;
		}
		else
		{
			//if not root password
			xSemaphoreTake(uart_mutex,portMAX_DELAY);
			vUartTransmit(ENTER_KEY);
			vUartTransmitString_program(PSTR("INVALID PASSWORD ......"));
			vUartTransmit(ENTER_KEY);vUartTransmit(ENTER_KEY);
			xSemaphoreGive(uart_mutex);
			return;
		}
	}
	else
	{
		//error
		xSemaphoreTake(uart_mutex,portMAX_DELAY);
		vUartTransmit(ENTER_KEY);
		vUartTransmitString_program(PSTR("INVALID PASSWORD ......"));
		vUartTransmit(ENTER_KEY);vUartTransmit(ENTER_KEY);
		xSemaphoreGive(uart_mutex);
	}
	
	
}
//--------------------------------------------------------------
static void vTerminal_logged_off_state(void)
{
	u8 returned_key;
	while(1)
	{
		vUartTransmit(ENTER_KEY);
		vUartTransmitString_program(PSTR("###"));
		returned_key=u8Get_at_char();
		if(returned_key == AT_KEY)
		{
			//change to check password state
			task_state=TERMINAL_TASK_CHECK_ADMIN_PASS_STATE;
			break;
		}
	}		
}
//----------------------------------------------------------
static void vTerminal_check_admin_pass_state(void)
{
	u8 returned_key;
	
	xSemaphoreTake(uart_mutex,portMAX_DELAY);
	
	vUartTransmit(ENTER_KEY);
	vUartTransmitString_program(PSTR("ENTER ADMIN PASSWORD : "));
	xSemaphoreGive(uart_mutex);
	//check
	returned_key=u8Get_user_password();
	if(returned_key==PASSWORD_FOUND)
	{
		if (current_admin.type>ADMIN_USER)
		{
			//error
			invalid_password_print();
		}
		else
		{
			xSemaphoreTake(uart_mutex,portMAX_DELAY);
			vUartTransmit(ENTER_KEY);
			vUartTransmitString_program(PSTR("LOGGING IN ....... "));
			vUartTransmit(ENTER_KEY);
			xSemaphoreGive(uart_mutex);
			admin_invalid_count=0;
			task_state=TERMINAL_TASK_MENU_STATE;
		}
	}
	else if(returned_key ==TERMINAL_TIME_OUT)
	{
		task_state=TERMINAL_TASK_LOGGED_OFF_STATE;
	}
	else
	{
		//error
		invalid_password_print();
	}
	
}
//-----------------------------------------------------------------
//check if root user
void vTerminal_users_menu_state(void)
{
	u8 error;
	if (current_admin.type>ROOT_USER)
	{
		//print enter root user passsword
		xSemaphoreTake(uart_mutex,portMAX_DELAY);
		vUartTransmitString_program(PSTR("ENTER ROOT USER PASSWORD : "));
		xSemaphoreGive(uart_mutex);
		error=u8Get_user_password();
		if(error ==PASSWORD_FOUND)
		{
			if(current_admin.type ==ROOT_USER)
			{
				vMenu_users_options_print();
				menu_users_choose();
			}
			else
			{
				xSemaphoreTake(uart_mutex,portMAX_DELAY);
				vUartTransmit(ENTER_KEY);
				vUartTransmitString_program(PSTR("NOT ROOT PASSWORD .... "));
				vUartTransmit(ENTER_KEY);
				xSemaphoreGive(uart_mutex);
				task_state=TERMINAL_TASK_MENU_STATE;
				//print enter root password
			}
		}
		else
		{
			//print enter root password
			xSemaphoreTake(uart_mutex,portMAX_DELAY);
			vUartTransmit(ENTER_KEY);
			vUartTransmitString_program(PSTR("WRONG ROOT PASSWORD .... "));
			vUartTransmit(ENTER_KEY);
			xSemaphoreGive(uart_mutex);
			task_state=TERMINAL_TASK_MENU_STATE;
		}
	}
	else
	{
		//if root
		vMenu_users_options_print();
		menu_users_choose();
	}
	
}
//-------------------------------------------------------------
static void vTerminal_locked_state(void)
{
	u8 returned_key;
	static u8 invalid_locked_count=0;
	xSemaphoreTake(uart_mutex,portMAX_DELAY);
	
	vUartTransmit(ENTER_KEY);
	vUartTransmit(ENTER_KEY);
	vUartTransmitString_program(PSTR("TERMINAL LOCKED .... "));
	vUartTransmit(ENTER_KEY);
	vUartTransmitString_program(PSTR("ENTER ROOT PASSWORD TO UNLOCK : "));
	
	xSemaphoreGive(uart_mutex);
	
	returned_key=u8Get_user_password();
	if(returned_key ==PASSWORD_FOUND)
	{
		//check password
		if(current_admin.type ==ROOT_USER)
		{
			invalid_locked_count=0;
			task_state=TERMINAL_TASK_LOGGED_OFF_STATE;
		}
	}
	else
	{
		//call mina badrous :D
		invalid_locked_count++;
		if(invalid_locked_count ==3)
		{
			xSemaphoreTake(uart_mutex,portMAX_DELAY);
			
			vUartTransmit(ENTER_KEY);
			vUartTransmit(ENTER_KEY);
			vUartTransmitString_program(PSTR("SYSTEM LOCKED PLEASE CONTACT THE DEVELOPER "));
			vUartTransmit(ENTER_KEY);
			
			xSemaphoreGive(uart_mutex);
			//save state in eeprom 
			vEXTERNAL_EEPROM_lock_state();
			
			//stop os and stuck
			
			//vTaskSuspendAll();
			//vTaskSuspend(keypad_task_handler);
			//vTaskResumeAll();
			
			while(g_unlock_system_flag==0)
			{
				vTaskDelay(500);
			}
			invalid_locked_count=0;
			admin_invalid_count=0;
			g_unlock_system_flag=0;
			vEXTERNAL_EEPROM_unlock_state();
			//xTaskResumeAll();
			//vTaskResume(keypad_task_handler);
			task_state=TERMINAL_TASK_LOGGED_OFF_STATE;
		}
	}
}
