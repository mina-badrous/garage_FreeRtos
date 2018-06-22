/*
 * motor_task.h
 *
 * Created: 02/06/2018 15:05:56
 *  Author: BADROUS
 */ 
//---------------------------------------------------------------------
#ifndef MOTOR_TASK_H_
#define MOTOR_TASK_H_
//--------------------------------------------------------------------
#include "main.h"
#include "motor_driver_Cfg.h"
//--------------------------------------------------------------------
#define OPENING_GARAGE_DELAY 5000
#define CLOSING_GARAGE_DELAY 5000
#define MOTOR_TASK_QUEUE_SIZE 2
#define MOTOR_TASK_QUEUE_DATA_SIZE 1
//--------------------------------------------------------------
typedef enum
{
	MOTOR_TASK_CLOSED_STATE=0,
	MOTOR_TASK_OPENING_GARAGE_STATE,
	MOTOR_TASK_OPENED_GARAGE_STATE,
	MOTOR_TASK_CLOSING_GARAGE_STATE
} motor_task_state_t;
//-------------------------------------------------------
motor_task_state_t xGet_garage_task_state(void);
void vSet_garage_task_state(motor_task_state_t state);

void vMotor_task(void *pvParameters);
//-------------------------------------------------------
void vMotor_task_q_set(xQueueHandle key);
xQueueHandle xMotor_task_q_get(void);
//-------------------------------------------------------
#endif /* MOTOR_TASK_H_ */