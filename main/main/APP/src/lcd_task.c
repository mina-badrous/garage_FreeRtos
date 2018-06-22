/*
 * lcd_task.c
 *
 * Created: 31/05/2018 4:53:40
 *  Author: BADROUS
 */ 
//----------------------------------------------------------------
#include "lcd_task.h"
//---------------------------------------------------------------
static xQueueHandle g_lcd_q;

static xSemaphoreHandle g_lcd_task_resume_semaphore=NULL;
static lcd_task_state_t lcd_state=LCD_TASK_INIT_STATE;
//---------------------------------------------------------------------
void vlcd_task(void *pvParameters)
{
	portBASE_TYPE que_state;
	u8 invalid_password_count=0;
	u8 led_state=0;
	u8 motor_state=0;
	u8 password_index=0;
	u8 id;
	user_t user;
	u8 password_array[MAX_PASSWORD_CHAR]={0,0,0,0};	
	unsigned char data,index;
	
	//init
	vEcu_LcdInit(&lcd_param);
	lcd_state=LCD_TASK_WELCOME_STATE;
	//do task
	while (1)
	{
		switch(lcd_state)
		{
			case LCD_TASK_WELCOME_STATE :
										//change the state of led task
										led_state=LED_TASK_IDLE_STATE;
										xQueueSendToBack(xLed_task_q_get(),(void*)&led_state,20);
										//print welcome					
										u8Ecu_LcdGoToXy(0,5);
										u8Ecu_LcdSendString("WELCOME");
										//block till * and change state to enter state if *
										que_state= xQueueReceive( xkeypad_task_q_get(),&data,portMAX_DELAY);
										if(que_state ==pdPASS)
										{
											//print on lcd
											if(data == '#')
											{
												vEcu_LcdClearDisplay();
												u8Ecu_LcdSendString("ENTER PASSWORD:");
												u8Ecu_LcdGoToXy(1,0);
												password_index=0;
												lcd_state=LCD_TASK_ENTER_PASSWORD_STATE;
											}
											
										}
										break;
			case LCD_TASK_ENTER_PASSWORD_STATE :
												//receive from que and print to lcd if not reciecved till 5 second reset
												que_state= xQueueReceive( xkeypad_task_q_get(),&data,(5000/ portTICK_PERIOD_MS));//portMAX_DELAY
												if(que_state ==pdPASS)
												{
												//print on lcd
													if(data == '*')
													{
														//decrement index 
														password_index--;
														//check -1 condition
														if(password_index==255)
														{
															password_index=0;
														}
														else
														{	//clear last char if any
															vEcu_LcdRemovePassword();
														}
													}
													else if(data == '#')
													{
														//processing
														for(index=0 ;index<MAX_USER_PASSWORD;index++)
														{
															user.password[index]=password_array[index];
														}
														lcd_state=LCD_TASK_CHECK_PASSWORD_STATE;
													}
													else
													{
														//any char
														if((password_index >=0) && (password_index <MAX_PASSWORD_CHAR))
														{
															vEcu_LcdSendPassword(data);
															//save pass
															password_array[password_index]=data;
															//increment index
															password_index++;
															//check max index
														}
														
													}
												}
												else
												{
													//time out finished
													vEcu_LcdClearDisplay();
													lcd_state=LCD_TASK_WELCOME_STATE;
												}
												break;																											
			case LCD_TASK_CHECK_PASSWORD_STATE :
												//que_state= xQueueReceive( xpassword_check_task_q_get(),&password_check,portMAX_DELAY);
												xSemaphoreTake(i2c_mutex,portMAX_DELAY);
												data=u8EXTERNAL_EEPROM_search_by_password(user.password,user.name,&id,&(user.type));
												xSemaphoreGive(i2c_mutex);
												if(data ==PASSWORD_FOUND)
												{
													//valid password
													vEcu_LcdClearDisplay();
													u8Ecu_LcdGoToXy(0,0);
													u8Ecu_LcdSendString("WELCOME");
													u8Ecu_LcdGoToXy(1,5);
													u8Ecu_LcdSendString((char * )user.name);
													vTaskDelay(1500 / portTICK_PERIOD_MS);
													//change the state of led task
													led_state=LED_TASK_GARAGE_OPENED_STATE;
													xQueueSendToBack(xLed_task_q_get(),(void*)&led_state,20);
													//signal the log task to log
													xQueueSendToBack(xLog_task_q_get(),(void*)&user,20);
													
													
													//signal motor task and wait till finish
													motor_state=MOTOR_TASK_OPENING_GARAGE_STATE;
													xQueueSendToBack(xMotor_task_q_get(),(void*)&motor_state,20);
													vEcu_LcdClearDisplay();
													u8Ecu_LcdGoToXy(0,5);
													u8Ecu_LcdSendString("OPENINIG");
																										 
													vTaskDelay(OPENING_GARAGE_DELAY / portTICK_PERIOD_MS);
													
													motor_state=MOTOR_TASK_CLOSING_GARAGE_STATE;
													xQueueSendToBack(xMotor_task_q_get(),(void*)&motor_state,20);
													vEcu_LcdClearDisplay();
													u8Ecu_LcdGoToXy(0,5);
													u8Ecu_LcdSendString("CLOSING");	
																									
													vTaskDelay(CLOSING_GARAGE_DELAY / portTICK_PERIOD_MS);
													
													motor_state=MOTOR_TASK_CLOSED_STATE;
													xQueueSendToBack(xMotor_task_q_get(),(void*)&motor_state,20);													
													invalid_password_count=0;
													vEcu_LcdClearDisplay();
													lcd_state=LCD_TASK_WELCOME_STATE;
												}
												else
												{
													//invalid password
													invalid_password_count++;
													//change the state of led task
													//signal the log task to log
													//change user type to undefined type
													user.type=INVALID_USER;
													xQueueSendToBack(xLog_task_q_get(),(void*)&user,20);
													//change lcd data
													for(index=0;index<3;index++)
													{
														vEcu_LcdClearDisplay();
														u8Ecu_LcdGoToXy(0,0);
														u8Ecu_LcdSendString("INVALID PASSWORD");
														vTaskDelay(800/ portTICK_PERIOD_MS);
													}				
													vEcu_LcdClearDisplay();
													u8Ecu_LcdGoToXy(0,1);
													u8Ecu_LcdSendString("WRONG ENTRY");
													u8Ecu_LcdGoToXy(0,13);
													vEcu_LcdSendChar(invalid_password_count+'0');
													vTaskDelay(800/ portTICK_PERIOD_MS);
													if(invalid_password_count==LCD_TASK_MAX_INVALID_PASSWORD_COUNT)
													{
														//change the state of led task
														led_state=LED_TASK_ALERT_STATE;
														xQueueSendToBack(xLed_task_q_get(),(void*)&led_state,20);
														vEcu_LcdClearDisplay();
														u8Ecu_LcdGoToXy(0,5);
														u8Ecu_LcdSendString("LOCKED");
														lcd_state=LCD_TASK_LOCKED_STATE;
													}
													else
													{
														//invalid password < 3
														vEcu_LcdClearDisplay();
														lcd_state=LCD_TASK_WELCOME_STATE;
													}
												}
												break;
			case LCD_TASK_LOCKED_STATE :
										//wait to signal from terminal task to return to welcome state
										xSemaphoreTake( g_lcd_task_resume_semaphore,portMAX_DELAY);
										invalid_password_count=0;
										vEcu_LcdClearDisplay();
										vTaskDelay(500/portTICK_PERIOD_MS);
										xQueueReceive( xkeypad_task_q_get(),&data,0);
										lcd_state=LCD_TASK_WELCOME_STATE;
										break;						
			default:break;
		}

		vTaskDelay( 50 );
	
	}

}
//-----------------------------------------------------------
void vlcd_task_q_set(xQueueHandle key)
{
	g_lcd_q=key;
}
//-----------------------------------------------------
xQueueHandle xlcd_task_q_get(void)
{
	return g_lcd_q;
}
//----------------------------------------------------
void vlcd_task_resume_semaphore_set(xSemaphoreHandle key)
{
	g_lcd_task_resume_semaphore=key;
}
//-----------------------------------------------------
xSemaphoreHandle xlcd_task_resume_semaphore_get(void)
{
	return g_lcd_task_resume_semaphore;
}
//--------------------------------------------
lcd_task_state_t xlcd_task_get_state(void)
{
	return lcd_state;
}