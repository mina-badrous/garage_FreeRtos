/*
 * led_task.h
 *
 * Created: 02/06/2018 13:21:18
 *  Author: BADROUS
 */ 
//------------------------------------------------------------------
#ifndef LED_TASK_H_
#define LED_TASK_H_
//-----------------------------------------------------------
#include "main.h"
#include "ecu_buzzer_Cfg.h"
#include "ecu_led_Cfg.h"
//---------------------------------------------------------
#define LED_TASK_QUEUE_SIZE 3
#define LED_TASK_QUEUE_DATA_SIZE 1
//-----------------------------------------------------------
typedef enum
{
	LED_TASK_IDLE_STATE=0,
	LED_TASK_GARAGE_OPENED_STATE,
	LED_TASK_ALERT_STATE
} led_task_state_t;
//-----------------------------------------------------------
void vled_task(void *pvParameters);
//------------------------------------------------------------------
void vLed_task_q_set(xQueueHandle key);
xQueueHandle xLed_task_q_get(void);
//------------------------------------------------------------------
#endif /* LED_TASK_H_ */