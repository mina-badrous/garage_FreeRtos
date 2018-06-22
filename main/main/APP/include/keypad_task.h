/*
 * keypad_task.h
 *
 * Created: 31/05/2018 4:54:22
 *  Author: BADROUS
 */ 
//--------------------------------------------------------------------------
#ifndef KEYPAD_TASK_H_
#define KEYPAD_TASK_H_
//----------------------------------------------------------------------
#include "main.h"
#include "ecu_keypad_Cfg.h"
//-----------------------------------------------------
#define KEYPAD_TASK_QUEUE_SIZE 1
#define KEYPAD_TASK_QUEUE_DATA_SIZE (sizeof(char))
//---------------------------------------------------------------
void vkeypad_task(void *pvParameters);
//-------------------------------------------------
xQueueHandle xkeypad_task_q_get(void);
void vkeypad_task_q_set(xQueueHandle key);
//-------------------------------------------------------
#endif /* KEYPAD_H_ */