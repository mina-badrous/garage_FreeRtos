/*
 * terminal_user_menu.c
 *
 * Created: 08/06/2018 2:54:40
 *  Author: BADROUS
 */ 
#include "terminal_user_menu.h"



void list_users(void)
{
	user_t user;
	u8 id,index;
	eeprom_memory_control_block_t memory;
	xSemaphoreTake(uart_mutex,portMAX_DELAY);
	
	
	vGet_current_MCB(&memory);
	
	vUartTransmit(ENTER_KEY);
	vUartTransmitString_program(PSTR("--------------------------------------------------"));
	vUartTransmit(ENTER_KEY);
	vUartTransmitString_program(PSTR("CURRENT USERS OF THE SYSTEM"));
	vUartTransmit(ENTER_KEY);
	vUartTransmitString_program(PSTR("--------------------------"));
	vUartTransmit(ENTER_KEY);
	
	for(id=0;id<EEPROM_MAX_USER;id++)
	{
		if(check_bit(memory.current_user_used_flag,id) ==1)
		{
			xSemaphoreTake(i2c_mutex,portMAX_DELAY);
			vEXTERNAL_EEPROM_get_user(&user,id);
			xSemaphoreGive(i2c_mutex);
			
			vUartTransmit(id + '0' + 1 );
			
			vUartTransmitString_program(PSTR(" - "));
			//print name
			for(index=0;index<MAX_USER_NAME;index++)
			{
				if((user.name[index]) !='\0')
				{
					vUartTransmit((user.name[index]));
					
				}
				else
				{
					break;
				}
			}
			//print type
			vUartTransmitString_program(PSTR("   -> "));
			if(user.type ==ROOT_USER)
			{
				vUartTransmitString_program(PSTR("ROOT USER "));
			}
			else if(user.type ==ADMIN_USER)
			{
				vUartTransmitString_program(PSTR("ADMIN USER "));
			}
			else
			{
				vUartTransmitString_program(PSTR("NORMAL USER "));
			}
			vUartTransmit(ENTER_KEY);
			vUartTransmitString_program(PSTR("------------------------------------------"));
			vUartTransmit(ENTER_KEY);	
		}
		
	}
	xSemaphoreGive(uart_mutex);
}
//------------------------------------------------------------------

void add_users(void)
{
	enum
	{
		ADD_USER_STATE_ADD_NAME=0,
		ADD_USER_STATE_ADD_PASS,
		ADD_USER_STATE_ADD_TYPE,
		ADD_USER_ADD_IN_MEMORY
	}	func_state;
	enum
	{
		ADD_USER_NO_ERROR=0,
		ADD_USER_INAVLID_NAME_ERROR,
		ADD_USER_NAME_USED_ERROR,
		ADD_USER_INAVLID_PASS_ERROR,
		ADD_USER_INAVLID_PASS_USED,
		ADD_USER_INAVLID_TYPE_ERROR,
		ADD_USER_TIMEOUT_ERROR
	} func_error_state;
	u8 result;
	user_t user;
	u8 id,type;
	u8 name[10];
	func_state=ADD_USER_STATE_ADD_NAME;
	func_error_state=ADD_USER_NO_ERROR;
	
	xSemaphoreTake(uart_mutex,portMAX_DELAY);
	
	vUartTransmitString_program(PSTR("--------------------------------"));
	vUartTransmit(ENTER_KEY);
	while(1)
	{
		switch(func_state)
		{
			case ADD_USER_STATE_ADD_NAME :
								vUartTransmitString_program(PSTR("ENTER USER NAME (MAX 9 CHAR) : "));
								result=get_user_name(&(user.name[0]));
								if(result == VALID_NAME)
								{
									//check if name is used
									xSemaphoreTake(i2c_mutex,portMAX_DELAY);
									result=u8EXTERNAL_EEPROM_search_by_name(user.name);
									xSemaphoreGive(i2c_mutex);
									if(result ==USER_NAME_NOT_FOUND)
									{
										func_error_state=ADD_USER_NO_ERROR;
										func_state=ADD_USER_STATE_ADD_PASS;
									}
									else
									{
										func_error_state=ADD_USER_NAME_USED_ERROR;
										break;
									}
				
								}
								else if(result==TERMINAL_RETURN_U8)
								{
									//print user must be 9 char long
									func_error_state=ADD_USER_INAVLID_NAME_ERROR;
									break;
								}
								else
								{
									//time out
									func_error_state=ADD_USER_TIMEOUT_ERROR;
									break;
								}
			case ADD_USER_STATE_ADD_PASS :
								vUartTransmit(ENTER_KEY);
								vUartTransmitString_program(PSTR("ENTER USER PASSWORD (4 DIGIT) : "));
								result=u8Get_user_password_arr(&(user.password[0]));
								if(result==PASSWORD_VALID)
								{
									xSemaphoreTake(i2c_mutex,portMAX_DELAY);
									result=u8EXTERNAL_EEPROM_search_by_password(user.password,name,&id,&type);
									xSemaphoreGive(i2c_mutex);
									if(result ==PASSWORD_NOT_FOUND)
									{
										func_state=ADD_USER_STATE_ADD_TYPE;
										func_error_state=ADD_USER_NO_ERROR;
									}
									else
									{
										func_error_state=ADD_USER_INAVLID_PASS_USED;
										break;
									}
				
								}
								else
								{
									//password not valid
									func_error_state=ADD_USER_INAVLID_PASS_ERROR;
									break;
								}
			case ADD_USER_STATE_ADD_TYPE :
								vUartTransmit(ENTER_KEY);
								vUartTransmitString_program(PSTR("ENTER USER TYPE (ROOT = 0) | (admin = 1) | (user = 2)  : "));
								result=get_u8(&(user.type));
								if(result == TERMINAL_RETURN_U8)
								{
									if(user.type <=2)
									{
										//add user in eeprom
										func_state=ADD_USER_ADD_IN_MEMORY;
										func_error_state=ADD_USER_NO_ERROR;
									}
									else
									{
										func_error_state=ADD_USER_INAVLID_TYPE_ERROR;
										break;
									}
								}
								else
								{
									//invalid type
									func_error_state=ADD_USER_TIMEOUT_ERROR;
									break;
								}
			case ADD_USER_ADD_IN_MEMORY	:
								xSemaphoreTake(i2c_mutex,portMAX_DELAY);
								result=u8EXTERNAL_EEPROM_add_new_user(&user);
								xSemaphoreGive(i2c_mutex);
								if(result !=NO_AVAILABLE_USER)
								{
									vUartTransmit(ENTER_KEY);
									vUartTransmitString_program(PSTR("USER ADDED SUCESSFULY ........ "));
									vUartTransmit(ENTER_KEY);
									func_error_state=ADD_USER_NO_ERROR;
									
									xSemaphoreGive(uart_mutex);
									return;
								}
								else
								{
									//print you reach max number of user
									vUartTransmit(ENTER_KEY);
									vUartTransmitString_program(PSTR("FAILED TO ADD NEW USER YOU REACH MAX NUMBER OF USER ......... "));
									vUartTransmit(ENTER_KEY);
									
									xSemaphoreGive(uart_mutex);
									return;
								}
			default:break;
		}
		switch(func_error_state)
		{
			case ADD_USER_NO_ERROR :break;
			case ADD_USER_INAVLID_NAME_ERROR :
										vUartTransmit(ENTER_KEY);
										vUartTransmitString_program(PSTR("USER NAME MUST BE < 9 DIGIT ......... "));
										vUartTransmit(ENTER_KEY);
										vUartTransmit(ENTER_KEY);
										break;
			case ADD_USER_NAME_USED_ERROR :
										vUartTransmit(ENTER_KEY);
										vUartTransmitString_program(PSTR("THIS USER NAME ALREADY USED "));
										vUartTransmit(ENTER_KEY);
										vUartTransmit(ENTER_KEY);
										break;
			case ADD_USER_INAVLID_PASS_ERROR :
										vUartTransmit(ENTER_KEY);
										vUartTransmitString_program(PSTR("PASSWORD MUST BE 4 DIGIT ......... "));
										vUartTransmit(ENTER_KEY);
										vUartTransmit(ENTER_KEY);
										break;
			case ADD_USER_INAVLID_PASS_USED :
										//password used
										vUartTransmit(ENTER_KEY);
										vUartTransmitString_program(PSTR("THIS PASSWORD ALREADY USED "));
										vUartTransmit(ENTER_KEY);
										vUartTransmit(ENTER_KEY);
										break;
			case ADD_USER_INAVLID_TYPE_ERROR :
										vUartTransmit(ENTER_KEY);
										vUartTransmitString_program(PSTR("WRONG USER TYPE ......... "));
										vUartTransmit(ENTER_KEY);
										vUartTransmit(ENTER_KEY);
										break;
			case ADD_USER_TIMEOUT_ERROR :
			
										xSemaphoreGive(uart_mutex);
										return;
			default:break;
		}
	}
}
//---------------------------
void edit_users(void)
{
	enum
	{
		EDIT_USER_STATE_ENTER_ID=0,
		EDIT_USER_CHOOSE_EDIT,
		EDIT_USER_NAME,
		EDIT_USER_PASS,
		EDIT_USER_TYPE,
		EDIT_USER_SAVE_IN_MEMORY
	}	func_state,func_last_state;
	enum
	{
		EDIT_USER_NO_ERROR=0,
		EDIT_USER_INAVLID_ID_ERROR,
		EDIT_USER_INVALID_OPTION_ERROR,
		EDIT_USER_NAME_INAVLID_ERROR,
		EDIT_USER_NAME_USED_ERROR,
		EDIT_USER_INAVLID_PASS_ERROR,
		EDIT_USER_INAVLID_PASS_USED,
		EDIT_USER_INAVLID_TYPE_ERROR,
		EDIT_USER_TIMEOUT_ERROR
	} func_error_state;
	
	u8 result,temp;
	user_t user;
	u8 id,type,index;
	u8 name[10];
	u8 pass[4];
	
	func_state=EDIT_USER_STATE_ENTER_ID;
	func_last_state=EDIT_USER_STATE_ENTER_ID;
	func_error_state=EDIT_USER_NO_ERROR;
	
	xSemaphoreTake(uart_mutex,portMAX_DELAY);
	
	vUartTransmitString_program(PSTR("--------------------------------"));
	vUartTransmit(ENTER_KEY);
	while(1)
	{
		switch(func_state)
		{
			case EDIT_USER_STATE_ENTER_ID :
										vUartTransmit(ENTER_KEY);
										vUartTransmitString_program(PSTR("ENTER USER ID : "));
										result=get_u8(&(id));
										if(result == TERMINAL_RETURN_U8)
										{
											//check user by id
											id=id-1;
											xSemaphoreTake(i2c_mutex,portMAX_DELAY);
											result=u8EXTERNAL_EEPROM_search_user_byID(id);
											xSemaphoreGive(i2c_mutex);
											if(result ==USER_FOUND)
											{
												//get user
												xSemaphoreTake(i2c_mutex,portMAX_DELAY);
												vEXTERNAL_EEPROM_get_user(&user,id);
												xSemaphoreGive(i2c_mutex);
												func_state=EDIT_USER_CHOOSE_EDIT;
												func_error_state=EDIT_USER_NO_ERROR;
											}
											else
											{
												func_error_state=EDIT_USER_INAVLID_ID_ERROR;
												break;
											}
										}
										else
										{
											//invalid type
											func_error_state=EDIT_USER_TIMEOUT_ERROR;
											break;
										}
			case EDIT_USER_CHOOSE_EDIT :
										vUartTransmit(ENTER_KEY);
										vUartTransmitString_program(PSTR("SELECT FIELD  (1 -> NAME) (2 -> PASS) (3 -> TYPE) : "));
										result=get_u8(&(type));
										if(result == TERMINAL_RETURN_U8)
										{
											if(type ==1)
											{
												//choose which
												func_state=EDIT_USER_NAME;
												func_error_state=EDIT_USER_NO_ERROR;
											}
											else if(type==2)
											{
												func_state=EDIT_USER_PASS;
												func_error_state=EDIT_USER_NO_ERROR;
											}
											else if(type==3)
											{
												func_state=EDIT_USER_TYPE;
												func_error_state=EDIT_USER_NO_ERROR;
											}
											else
											{
												func_error_state=EDIT_USER_INVALID_OPTION_ERROR;
												break;
											}
										}
										else
										{
											//invalid type
											func_error_state=EDIT_USER_TIMEOUT_ERROR;
											break;
										}
										break;
			case EDIT_USER_NAME :
									vUartTransmit(ENTER_KEY);
									vUartTransmitString_program(PSTR("ENTER USER NAME (MAX 9 CHAR) : "));
									result=get_user_name(&(name[0]));
									if(result == VALID_NAME)
									{
										//check if name is used
										xSemaphoreTake(i2c_mutex,portMAX_DELAY);
										result=u8EXTERNAL_EEPROM_search_by_name(name);
										xSemaphoreGive(i2c_mutex);
										if(result ==USER_NAME_NOT_FOUND)
										{
											//save name in the user id
											func_error_state=EDIT_USER_NO_ERROR;
											func_state=EDIT_USER_SAVE_IN_MEMORY;
											func_last_state=EDIT_USER_NAME;
											break;
										}
										else
										{
											func_error_state=EDIT_USER_NAME_USED_ERROR;
											break;
										}
				
									}
									else if(result==TERMINAL_RETURN_U8)
									{
										//print user must be 9 char long
										func_error_state=EDIT_USER_NAME_INAVLID_ERROR;
										break;
									}
									else
									{
										//time out
										func_error_state=EDIT_USER_TIMEOUT_ERROR;
										break;
									}
									break;
			case EDIT_USER_PASS :
									vUartTransmit(ENTER_KEY);
									vUartTransmitString_program(PSTR("ENTER USER PASSWORD (4 DIGIT) : "));
									result=u8Get_user_password_arr(&(pass[0]));
									if(result==PASSWORD_VALID)
									{
										xSemaphoreTake(i2c_mutex,portMAX_DELAY);
										result=u8EXTERNAL_EEPROM_search_by_password(pass,name,&temp,&type);
										xSemaphoreGive(i2c_mutex);
										if(result ==PASSWORD_NOT_FOUND)
										{
											func_state=EDIT_USER_SAVE_IN_MEMORY;
											func_error_state=EDIT_USER_NO_ERROR;
											func_last_state=EDIT_USER_PASS;
											break;
										}
										else
										{
											func_error_state=EDIT_USER_INAVLID_PASS_USED;
											break;
										}
				
									}
									else
									{
										//password not valid
										func_error_state=EDIT_USER_INAVLID_PASS_ERROR;
										break;
									}
									break;
			case EDIT_USER_TYPE :
									vUartTransmit(ENTER_KEY);
									vUartTransmitString_program(PSTR("ENTER USER TYPE (ROOT = 0) | (admin = 1) | (user = 2)  : "));
									result=get_u8(&(type));
									if(result == TERMINAL_RETURN_U8)
									{
										if(type <=2)
										{
											//add user in eeprom
											func_state=EDIT_USER_SAVE_IN_MEMORY;
											func_error_state=EDIT_USER_NO_ERROR;
											func_last_state=EDIT_USER_TYPE;
											break;
										}
										else
										{
											func_error_state=EDIT_USER_INAVLID_TYPE_ERROR;
											break;
										}
									}
									else
									{
										//invalid type
										func_error_state=EDIT_USER_TIMEOUT_ERROR;
										break;
									}
			case EDIT_USER_SAVE_IN_MEMORY :
									if(func_last_state ==EDIT_USER_NAME)
									{
										// id +name
										for(index=0;index<MAX_USER_NAME;index++)
										{
											user.name[index]=name[index];
										}
										xSemaphoreTake(i2c_mutex,portMAX_DELAY);
										vEXTERNAL_EEPROM_edit_user(&user,id);
										xSemaphoreGive(i2c_mutex);
									}
									else if(func_last_state ==EDIT_USER_PASS)
									{
										//pass +id
										for(index=0;index<MAX_USER_PASSWORD;index++)
										{
											user.password[index]=pass[index];
										}
										xSemaphoreTake(i2c_mutex,portMAX_DELAY);
										vEXTERNAL_EEPROM_edit_user(&user,id);
										xSemaphoreGive(i2c_mutex);
				
									}
									else if(func_last_state ==EDIT_USER_TYPE)
									{
										user.type=type;
										xSemaphoreTake(i2c_mutex,portMAX_DELAY);
										vEXTERNAL_EEPROM_edit_user(&user,id);
										xSemaphoreGive(i2c_mutex);
				
									}
									else
									{
										//error
										xSemaphoreGive(uart_mutex);
										return;
									}
									vUartTransmit(ENTER_KEY);
									vUartTransmitString_program(PSTR("DONE EDITING ......... "));
									vUartTransmit(ENTER_KEY);
									vUartTransmit(ENTER_KEY);
									
									xSemaphoreGive(uart_mutex);
									return;
			default :break;
		}
		switch(func_error_state)
		{
			case EDIT_USER_NO_ERROR :break;
			case EDIT_USER_INAVLID_ID_ERROR :
										vUartTransmit(ENTER_KEY);
										vUartTransmitString_program(PSTR("INVALID ID ......... "));
										vUartTransmit(ENTER_KEY);
										vUartTransmit(ENTER_KEY);
										break;
			case EDIT_USER_INVALID_OPTION_ERROR :
										vUartTransmit(ENTER_KEY);
										vUartTransmitString_program(PSTR("INVALID OPTION ......... "));
										vUartTransmit(ENTER_KEY);
										vUartTransmit(ENTER_KEY);
										break;
			case EDIT_USER_NAME_INAVLID_ERROR :
										vUartTransmit(ENTER_KEY);
										vUartTransmitString_program(PSTR("INVALID USER NAME ......... "));
										vUartTransmit(ENTER_KEY);
										vUartTransmit(ENTER_KEY);
										break;
			case EDIT_USER_NAME_USED_ERROR :
										vUartTransmit(ENTER_KEY);
										vUartTransmitString_program(PSTR("USER NAME USED  ......... "));
										vUartTransmit(ENTER_KEY);
										vUartTransmit(ENTER_KEY);
										break;
			case EDIT_USER_INAVLID_PASS_ERROR :
										vUartTransmit(ENTER_KEY);
										vUartTransmitString_program(PSTR("INVALID PASSWORD ......... "));
										vUartTransmit(ENTER_KEY);
										vUartTransmit(ENTER_KEY);
										break;
			case EDIT_USER_INAVLID_PASS_USED :
										vUartTransmit(ENTER_KEY);
										vUartTransmitString_program(PSTR("PASSWORD USED  ......... "));
										vUartTransmit(ENTER_KEY);
										vUartTransmit(ENTER_KEY);
										break;
			case EDIT_USER_INAVLID_TYPE_ERROR :
										vUartTransmit(ENTER_KEY);
										vUartTransmitString_program(PSTR("WRONG USER TYPE ......... "));
										vUartTransmit(ENTER_KEY);
										vUartTransmit(ENTER_KEY);
										break;
			case EDIT_USER_TIMEOUT_ERROR :
											xSemaphoreGive(uart_mutex);
											return;
			default:break;
		}
	}
}
//---------------------------------------------------------------------------
void delete_users(void)
{
	u8 result,id;
	
	enum
	{
		INPUT_STATE,
		DELETE_STATE
	} func_state;
	
	enum
	{
		NO_ERROR,
		INVALID_INPUT_ERROR,
		UT_ERROR,
		ROOT_USER_ERROR,
		TIME_OUT_ERROR
	} func_error;
	
	xSemaphoreTake(uart_mutex,portMAX_DELAY);
	
	vUartTransmit(ENTER_KEY);
	vUartTransmitString_program(PSTR("--------------------------------------------------"));
	xSemaphoreGive(uart_mutex);
	func_state=INPUT_STATE;
	func_error=NO_ERROR;
	while(1)
	{
		switch(func_state)
		{
			case INPUT_STATE :
								xSemaphoreTake(uart_mutex,portMAX_DELAY);
								vUartTransmit(ENTER_KEY);
								vUartTransmitString_program(PSTR("ENTER ID NUMBER : "));
								xSemaphoreGive(uart_mutex);
								result=get_u8(&(id));
								if(result == TERMINAL_RETURN_U8)
								{
									//check user by id
									id=id-1;
									if(id ==0)
									{
										func_error=ROOT_USER_ERROR;
										break;
									}
									if(id >=EEPROM_MAX_USER)
									{
										func_error=INVALID_INPUT_ERROR;
										break;
									}
									xSemaphoreTake(i2c_mutex,portMAX_DELAY);
									result=u8EXTERNAL_EEPROM_search_user_byID(id);
									xSemaphoreGive(i2c_mutex);
									if(result ==USER_FOUND)
									{
										//choose which
										func_state=DELETE_STATE;
										func_error=NO_ERROR;
									}
									else
									{
										func_error=INVALID_INPUT_ERROR;
										break;
									}
								}
								else
								{
									//invalid type
									func_error=TIME_OUT_ERROR;
									break;
								}
								break;
			case DELETE_STATE :
								xSemaphoreTake(i2c_mutex,portMAX_DELAY);
								vEXTERNAL_EEPROM_delete_user(id);
								xSemaphoreGive(i2c_mutex);
								func_error=NO_ERROR;
								
								xSemaphoreTake(uart_mutex,portMAX_DELAY);
								vUartTransmit(ENTER_KEY);
								vUartTransmitString_program(PSTR("USER DELETED successfully .........."));
								vUartTransmit(ENTER_KEY);
								vUartTransmit(ENTER_KEY);
								xSemaphoreGive(uart_mutex);
								return;
			default:break;
		}
		switch(func_error)
		{
			case NO_ERROR :break;
			case INVALID_INPUT_ERROR :
			case ROOT_USER_ERROR :
								vUartTransmit(ENTER_KEY);
								vUartTransmitString_program(PSTR("INVALID USER .........."));
								vUartTransmit(ENTER_KEY);
								vUartTransmit(ENTER_KEY);
								xSemaphoreGive(uart_mutex);
			case TIME_OUT_ERROR :
								//xSemaphoreGive(uart_mutex);
								return;
			default:break;
		}
	}
}
//------------------------------
void factory_reset(void)
{
	vSet_terminal_task_state(TERMINAL_TASK_FACTORY_STATE);
	xSemaphoreTake(uart_mutex,portMAX_DELAY);
	vUartTransmit(ENTER_KEY);
	vUartTransmitString_program(PSTR("RESET SYSTEM DONE ......"));
	vUartTransmit(ENTER_KEY);
	vUartTransmit(ENTER_KEY);
	xSemaphoreGive(uart_mutex);
}