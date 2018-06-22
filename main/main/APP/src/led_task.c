/*
 * led_task.c
 *
 * Created: 02/06/2018 13:21:33
 *  Author: BADROUS
 */ 
//---------------------------------------------------------------
#include "led_task.h"
//---------------------------------------------------------------
static xQueueHandle g_led_task_q;
//----------------------------------------------------------------
void vled_task(void *pvParameters)
{
	led_task_state_t led_state=LED_TASK_IDLE_STATE;
	portBASE_TYPE que_state;
	u8 data;
	//init led and buzzer
	vEcu_led_init(&green_led_param);
	vEcu_led_init(&red_led_param);
	vEcu_Buzzer_init(&buzzer_param);
	while(1)
	{
		//receive from que if any and set state
		que_state= xQueueReceive(g_led_task_q,&data,0);
		if(que_state ==pdPASS)
		{
			led_state=data;
		}
		//switch state 
		switch(led_state)
		{
			case LED_TASK_IDLE_STATE :
												//toggle green led every 500 ms
												vEcu_toggle_led(&green_led_param);
												//clear led red 
												vEcu_set_led_off(&red_led_param);
												//buzzer stop
												vEcu_Buzzer_stop(&buzzer_param);
												vTaskDelay(2000);
												break;	
			case LED_TASK_GARAGE_OPENED_STATE :
												//toggle green led
												vEcu_toggle_led(&green_led_param); 
												//set red led on
												vEcu_set_led_on(&red_led_param);
												//buzzer stop
												vEcu_Buzzer_stop(&buzzer_param);
												vTaskDelay(1000);
												break;
			case LED_TASK_ALERT_STATE :
												//set green led off
												vEcu_set_led_off(&green_led_param);
												//toggle red_led
												vEcu_toggle_led(&red_led_param);
												//alert buzzer
												vEcu_Buzzer_toggle(&buzzer_param);
												vTaskDelay(500);
												break;
			default:break;
		}
		
	}
}
//-----------------------------------------------------------------
void vLed_task_q_set(xQueueHandle key)
{
	 g_led_task_q=key;
}
//-----------------------------------------------------
xQueueHandle xLed_task_q_get(void)
{
	return  g_led_task_q;
}
//----------------------------------------------------------------