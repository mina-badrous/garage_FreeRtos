/*
 * terminal_main_menu.c
 *
 * Created: 08/06/2018 2:53:38
 *  Author: BADROUS
 */ 
#include "terminal_main_menu.h"





 void unlock_system(void)
{
	lcd_task_state_t state=xlcd_task_get_state();
	if(state != LCD_TASK_LOCKED_STATE)
	{
		xSemaphoreTake(uart_mutex,portMAX_DELAY);
		vUartTransmitString_program(PSTR("THE SYSTEM IS NOT LOCKED ......"));
		vUartTransmit(ENTER_KEY);
		xSemaphoreGive(uart_mutex);
	}
	else
	{
		xSemaphoreTake(uart_mutex,portMAX_DELAY);
		vUartTransmitString_program(PSTR("UNLOCKING THE SYSTEM ......"));
		vUartTransmit(ENTER_KEY);
		xSemaphoreGive(uart_mutex);
		
		xSemaphoreGive( xlcd_task_resume_semaphore_get() );
	}
	
}
//--------------------------------------------------------------------------
 void open_garage(void)
{
	u8 led_state;
	motor_task_state_t garage_state=xGet_garage_task_state();
	if(garage_state ==MOTOR_TASK_CLOSED_STATE)
	{
		//set led state to opened state
		led_state=LED_TASK_GARAGE_OPENED_STATE;
		xQueueSendToBack(xLed_task_q_get(),(void*)&led_state,20);
		garage_state=MOTOR_TASK_OPENING_GARAGE_STATE;
		
		xQueueSendToBack(xMotor_task_q_get(),(void*)&garage_state,portMAX_DELAY);
		
		xSemaphoreTake(uart_mutex,portMAX_DELAY);
		vUartTransmit(ENTER_KEY);
		vUartTransmitString_program(PSTR("OPENING THE GARAGE ......"));
		vUartTransmit(ENTER_KEY);
		xSemaphoreGive(uart_mutex);
		
		vTaskDelay(OPENING_GARAGE_DELAY / portTICK_PERIOD_MS);
		garage_state=MOTOR_TASK_OPENED_GARAGE_STATE;
		xQueueSendToBack(xMotor_task_q_get(),(void*)&garage_state,portMAX_DELAY);
	}
	else
	{
		xSemaphoreTake(uart_mutex,portMAX_DELAY);
		vUartTransmit(ENTER_KEY);
		vUartTransmitString_program(PSTR("GARAGE HAS ALREADY BEEN OPENED ......"));
		vUartTransmit(ENTER_KEY);
		xSemaphoreGive(uart_mutex);
	}


}
//--------------------------------------------------------------------------
 void close_garage(void)
{
	u8 led_state=0;
	motor_task_state_t garage_state=xGet_garage_task_state();
	
	if(garage_state ==MOTOR_TASK_OPENED_GARAGE_STATE)
	{
		//set led state to off
		led_state=LED_TASK_IDLE_STATE;
		xQueueSendToBack(xLed_task_q_get(),(void*)&led_state,20);
		//set motor state
		garage_state=MOTOR_TASK_CLOSING_GARAGE_STATE;
		xQueueSendToBack(xMotor_task_q_get(),(void*)&garage_state,portMAX_DELAY);
		
		xSemaphoreTake(uart_mutex,portMAX_DELAY);
		vUartTransmit(ENTER_KEY);
		vUartTransmitString_program(PSTR("CLOSING THE GARAGE ......"));
		vUartTransmit(ENTER_KEY);
		xSemaphoreGive(uart_mutex);
		
		vTaskDelay(CLOSING_GARAGE_DELAY / portTICK_PERIOD_MS);
		garage_state=MOTOR_TASK_CLOSED_STATE;
		xQueueSendToBack(xMotor_task_q_get(),(void*)&garage_state,portMAX_DELAY);
	}
	else
	{
		xSemaphoreTake(uart_mutex,portMAX_DELAY);
		vUartTransmit(ENTER_KEY);
		vUartTransmitString_program(PSTR("GARAGE IS CLOSED..............."));
		vUartTransmit(ENTER_KEY);
		xSemaphoreGive(uart_mutex);
	}

}
//--------------------------------------------------------------------------
 void set_time(void)
{
	rtc_time_t time;
	u8 temp,first,second;
	enum
	{
		SET_HOUR_STATE,
		SET_MINUTE_STATE,
		SET_AM_PM_STATE,
		SET_DATE_STATE,
		SET_MONTH_STATE,
		SET_YEAR_STATE,
		SAVE_TIME_STATE,
	} func_state;
	enum
	{
		NO_ERROR,
		INVALID_HOUR_ERROR,
		INVALID_MINUTE_ERROR,
		INVALID_AM_ERROR,
		INVALID_DATE_ERROR,
		INVALID_MONTH_ERROR,
		INVALID_YEAR_ERROR,
		TIMEOUT_ERROR
	} func_error;
	
	func_state=SET_HOUR_STATE;
	func_error=NO_ERROR;
	
	xSemaphoreTake(uart_mutex,portMAX_DELAY);
	
	vUartTransmitString_program(PSTR("--------------------------------"));
	vUartTransmit(ENTER_KEY);
	while(1)
	{
		switch(func_state)
		{
			case SET_HOUR_STATE :
			vUartTransmit(ENTER_KEY);
			vUartTransmitString_program(PSTR("ENTER HOUR  : "));
			temp=get_u8(&(time.hours.raw_data));
			if(temp == TERMINAL_RETURN_U8)
			{
				if(time.hours.raw_data <=12 && time.hours.raw_data !=0)
				{
					//check >9
					if(time.hours.raw_data <9)
					{
						first=time.hours.raw_data;
						time.hours.h12.SELECT_12_24=RTC_12_FORMAT;
						time.hours.h12.first_digit=first;
						time.hours.h12.second_digit=0;
					}
					else
					{
						first=time.hours.raw_data%10;
						second=time.hours.raw_data/10;
						time.hours.h12.SELECT_12_24=RTC_12_FORMAT;
						time.hours.h12.first_digit=first;
						time.hours.h12.second_digit=second;
					}
					func_state=SET_MINUTE_STATE;
					func_error=NO_ERROR;
				}
				else
				{
					func_error=INVALID_HOUR_ERROR;
					break;
				}
			}
			else
			{
				//invalid type
				func_error=TIMEOUT_ERROR;
				break;
			}
			case SET_MINUTE_STATE :
			vUartTransmit(ENTER_KEY);
			vUartTransmitString_program(PSTR("ENTER MINUTE  : "));
			temp=get_u8(&(time.minutes.raw_data));
			if(temp == TERMINAL_RETURN_U8)
			{
				if(time.minutes.raw_data <=59)
				{
					//check >9
					if(time.minutes.raw_data <9)
					{
						first=time.minutes.raw_data;
						time.minutes.bcd_data.first_digit=first;
						time.minutes.bcd_data.second_digit=0;
					}
					else
					{
						first=time.minutes.raw_data%10;
						second=time.minutes.raw_data/10;
						time.minutes.bcd_data.first_digit=first;
						time.minutes.bcd_data.second_digit=second;
					}
					func_state=SET_AM_PM_STATE;
					func_error=NO_ERROR;
				}
				else
				{
					func_error=INVALID_MINUTE_ERROR;
					break;
				}
			}
			else
			{
				//invalid type
				func_error=TIMEOUT_ERROR;
				break;
			}
			case SET_AM_PM_STATE :
			vUartTransmit(ENTER_KEY);
			vUartTransmitString_program(PSTR("ENTER (1 -> AM ) (2 -> PM)  : "));
			temp=get_u8(&(first));
			if(temp == TERMINAL_RETURN_U8)
			{
				if(first ==1)
				{
					time.hours.h12.SELECT_AM_PM=RTC_AM;
					func_state=SET_DATE_STATE;
					func_error=NO_ERROR;
				}
				else if(first ==2)
				{
					time.hours.h12.SELECT_AM_PM=RTC_PM;
					func_state=SET_DATE_STATE;
					func_error=NO_ERROR;
				}
				else
				{
					func_error=INVALID_AM_ERROR;
					break;
				}
			}
			else
			{
				//invalid type
				func_error=TIMEOUT_ERROR;
				break;
			}
			case SET_DATE_STATE :
			vUartTransmit(ENTER_KEY);
			vUartTransmitString_program(PSTR("ENTER DATE  : "));
			temp=get_u8(&(time.date.raw_data));
			if(temp == TERMINAL_RETURN_U8)
			{
				if(time.date.raw_data <=31 && time.date.raw_data !=0)
				{
					//check >9
					if(time.date.raw_data <9)
					{
						first=time.date.raw_data;
						time.date.bcd_data.first_digit=first;
						time.date.bcd_data.second_digit=0;
					}
					else
					{
						first=time.date.raw_data%10;
						second=time.date.raw_data/10;
						time.date.bcd_data.first_digit=first;
						time.date.bcd_data.second_digit=second;
					}
					func_state=SET_MONTH_STATE;
					func_error=NO_ERROR;
				}
				else
				{
					func_error=INVALID_DATE_ERROR;
					break;
				}
			}
			else
			{
				//invalid type
				func_error=TIMEOUT_ERROR;
				break;
			}
			case SET_MONTH_STATE :
			vUartTransmit(ENTER_KEY);
			vUartTransmitString_program(PSTR("ENTER MONTH  : "));
			temp=get_u8(&(time.month.raw_data));
			if(temp == TERMINAL_RETURN_U8)
			{
				if(time.month.raw_data <=12 && time.month.raw_data !=0)
				{
					//check >9
					if(time.month.raw_data <9)
					{
						first=time.month.raw_data;
						time.month.bcd_data.first_digit=first;
						time.month.bcd_data.second_digit=0;
					}
					else
					{
						first=time.month.raw_data%10;
						second=time.month.raw_data/10;
						time.month.bcd_data.first_digit=first;
						time.month.bcd_data.second_digit=second;
					}
					func_state=SET_YEAR_STATE;
					func_error=NO_ERROR;
				}
				else
				{
					func_error=INVALID_MONTH_ERROR;
					break;
				}
			}
			else
			{
				//invalid type
				func_error=TIMEOUT_ERROR;
				break;
			}
			case SET_YEAR_STATE :
			vUartTransmit(ENTER_KEY);
			vUartTransmitString_program(PSTR("ENTER YEAR  : "));
			temp=get_u8(&(time.year.raw_data));
			if(temp == TERMINAL_RETURN_U8)
			{
				if(time.year.raw_data <=99)
				{
					//check >9
					if(time.year.raw_data <9)
					{
						first=time.year.raw_data;
						time.year.bcd_data.first_digit=first;
						time.year.bcd_data.second_digit=0;
					}
					else
					{
						first=time.year.raw_data%10;
						second=time.year.raw_data/10;
						time.year.bcd_data.first_digit=first;
						time.year.bcd_data.second_digit=second;
					}
					func_state=SAVE_TIME_STATE;
					func_error=NO_ERROR;
				}
				else
				{
					func_error=INVALID_YEAR_ERROR;
					break;
				}
			}
			else
			{
				//invalid type
				func_error=TIMEOUT_ERROR;
				break;
			}
			case SAVE_TIME_STATE :
							xSemaphoreTake(i2c_mutex,portMAX_DELAY);
							u8Rtc_Set_Time(&time);
							xSemaphoreGive(i2c_mutex);
							vUartTransmit(ENTER_KEY);
							vUartTransmitString_program(PSTR("DONE TIME IS SET ....... "));
							vUartTransmit(ENTER_KEY);
							xSemaphoreGive(uart_mutex);
							return;
			default:break;
		}
		switch(func_error)
		{
			case NO_ERROR :break;
			case INVALID_HOUR_ERROR:
			case INVALID_MINUTE_ERROR :
			case INVALID_AM_ERROR :
			case INVALID_DATE_ERROR:
			case INVALID_MONTH_ERROR:
			case INVALID_YEAR_ERROR :
			vUartTransmit(ENTER_KEY);
			vUartTransmitString_program(PSTR("INVALID INPUT ....... "));
			vUartTransmit(ENTER_KEY);
			break;
			case TIMEOUT_ERROR :
			vSet_terminal_task_state(TERMINAL_TASK_LOGGED_OFF_STATE);
			xSemaphoreGive(uart_mutex);
			return;
			default :break;
		}
	}
	
}