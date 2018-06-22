/*
 * log_task.h
 *
 * Created: 05/06/2018 17:42:43
 *  Author: BADROUS
 */ 
//-------------------------------------------------------------
#ifndef LOG_TASK_H_
#define LOG_TASK_H_
//---------------------------------------------------------
#include "main.h"
#include "rtc.h"
#include "uart.h"
#include "terminal_task.h"
//----------------------------------------------------------
#define LOG_TASK_QUEUE_SIZE 2
#define LOG_TASK_QUEUE_DATA_SIZE (sizeof(user_t))
//----------------------------------------------------------
typedef enum
{
	LOG_TASK_VALID_PASSWORD=0,
	LOG_TASK_INVALID_PASSWORD
} log_task_password_valid_t;
//-----------------------------------------------------------
typedef enum
{
	LOG_TASK_IDLE_STATE=0,
	LOG_TASK_LOGGING_STATE
} log_task_state_t;
//---------------------------------------------------------------
void vLog_task(void *pvParameters);
//-----------------------------------------------------------
void vLog_task_q_set(xQueueHandle key);
xQueueHandle xLog_task_q_get(void);
//------------------------------------------------------------
#endif /* LOG_TASK_H_ */