/*
 * motor_task.c
 *
 * Created: 02/06/2018 15:05:44
 *  Author: BADROUS
 */ 
#include "motor_task.h"

static xQueueHandle g_motor_task_q;

static motor_task_state_t garage_state=MOTOR_TASK_CLOSED_STATE;

void vMotor_task(void *pvParameters)
{
	
	portBASE_TYPE que_state;
	u8 data;
	//init
	
	vMotor_driver_init(&motor_param);
	
	while(1)
	{
		//receive from que if any and set state
		que_state= xQueueReceive(g_motor_task_q,&data,portMAX_DELAY);
		if(que_state ==pdPASS)
		{
			garage_state=data;
		}
		//select state
		switch(garage_state)
		{
			case MOTOR_TASK_CLOSED_STATE :
											vMotor_driver_rotate(&motor_param,0,MOTOR_STOPED);
											//vTaskDelay(1000);
											break;
			case MOTOR_TASK_OPENING_GARAGE_STATE :
											vMotor_driver_rotate(&motor_param,50,MOTOR_ROTATE_CLOCKWISE);
											//vTaskDelay(OPENING_GARAGE_DELAY);
											break;
			case MOTOR_TASK_OPENED_GARAGE_STATE :
											vMotor_driver_rotate(&motor_param,0,MOTOR_STOPED);
											//vTaskDelay(1000);
											break;					
			case MOTOR_TASK_CLOSING_GARAGE_STATE :
											vMotor_driver_rotate(&motor_param,50,MOTOR_ROTATE_COUNTER_CLOCKWISE);
											//vTaskDelay(CLOSING_GARAGE_DELAY);
											break;			
			default:break;
		}
	}
}

//-----------------------------------------------------------------
void vMotor_task_q_set(xQueueHandle key)
{
	g_motor_task_q=key;
}
//-----------------------------------------------------
xQueueHandle xMotor_task_q_get(void)
{
	return  g_motor_task_q;
}
//--------------------------------------------------------------
motor_task_state_t xGet_garage_task_state(void)
{
	return garage_state;
}
void vSet_garage_task_state(motor_task_state_t state)
{
	garage_state=state;
}