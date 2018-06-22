/*
 * log_task.c
 *
 * Created: 05/06/2018 17:42:54
 *  Author: BADROUS
 */
//------------------------------------------------------------------ 
#include "log_task.h"
//--------------------------------------------------------------------
xQueueHandle g_log_q;
//-------------------------------------------------------------------
static void send_time(void);
static rtc_time_t time;
//-------------------------------------------------------------------
void vLog_task(void *pvParameters)
{
	user_t user;
	u8 index;
	portBASE_TYPE check;
	while(1)
	{
		//receive from que if any and set state
		check=xQueueReceive(g_log_q,&user,portMAX_DELAY);
		if(check !=pdPASS)
		{
			continue;
		}
		//take uart mutex
		check=xSemaphoreTake(uart_mutex,portMAX_DELAY);
		if(check !=pdPASS)
		{
			continue;
		}
		//capture time
		check=xSemaphoreTake(i2c_mutex,portMAX_DELAY);
		if(check !=pdPASS)
		{
			continue;
		}
		u8Rtc_Get_Time(&time);
		xSemaphoreGive(i2c_mutex);
		
		vUartTransmit(ENTER_KEY);
		if(user.type ==INVALID_USER)
		{
			//send invalid password with password try to enter
			vUartTransmitString_program(PSTR("SOME ONE WITH PASSWORD "));
			vUartTransmit(user.password[0]);vUartTransmit(user.password[1]);vUartTransmit(user.password[2]);vUartTransmit(user.password[3]);
			vUartTransmitString_program(PSTR(" TRY TO ENTER at "));
			send_time();
		}
		else
		{
			for(index=0 ;index<MAX_USER_NAME ;index++)
			{
				if(user.name[index] == '\0')
				{
					break;
				}
				vUartTransmit(user.name[index]);
			}
			vUartTransmitString_program(PSTR(" ENTER at "));
			send_time();
		}	
		xSemaphoreGive(uart_mutex);
	}
}
//---------------------------------------------------------------------------
void vLog_task_q_set(xQueueHandle key)
{
	g_log_q=key;
}
//-----------------------------------------------------
xQueueHandle xLog_task_q_get(void)
{
	return g_log_q;
}
//-------------------------------------------------------
void send_time(void)
{
	vUartTransmit(time.hours.h12.second_digit + '0');
	vUartTransmit(time.hours.h12.first_digit + '0');
	vUartTransmit(':');
	vUartTransmit(time.minutes.bcd_data.second_digit + '0');
	vUartTransmit(time.minutes.bcd_data.first_digit + '0');
	vUartTransmit(':');
	vUartTransmit(time.seconds.bcd_data.second_digit + '0');
	vUartTransmit(time.seconds.bcd_data.first_digit + '0');
	if(time.hours.h12.SELECT_AM_PM ==RTC_AM)
	{
		vUartTransmit(' ');
		vUartTransmit('A');
		vUartTransmit('M');
	}
	else
	{
		vUartTransmit(' ');
		vUartTransmit('P');
		vUartTransmit('M');
	}
	vUartTransmit(' ');
	vUartTransmit(time.date.bcd_data.second_digit + '0');
	vUartTransmit(time.date.bcd_data.first_digit + '0');
	vUartTransmit('-');
	vUartTransmit(time.month.bcd_data.second_digit + '0');
	vUartTransmit(time.month.bcd_data.first_digit + '0');
	vUartTransmit('-');
	vUartTransmit(time.year.bcd_data.second_digit + '0');
	vUartTransmit(time.year.bcd_data.first_digit + '0');
	vUartTransmit(ENTER_KEY);
}
//-----------------------------------------------------------------------------