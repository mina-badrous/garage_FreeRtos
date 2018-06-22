/*
 * lcd_task.h
 *
 * Created: 31/05/2018 4:53:23
 *  Author: BADROUS
 */ 
//--------------------------------------------------------------------
#ifndef LCD_TASK_H_
#define LCD_TASK_H_
//-------------------------------------------------------------------
#include "main.h"
#include "ecu_lcd_Cfg.h"
#include "keypad_task.h"
#include "led_task.h"
#include "motor_task.h"
#include "log_task.h"
//------------------------------------------------------------------
#define LCD_TASK_MAX_INVALID_PASSWORD_COUNT 3
#define LCD_TASK_VALID_PASSWORD 0
#define MAX_PASSWORD_CHAR 4
#define LCD_TASK_QUEUE_SIZE 1
#define LCD_TASK_QUEUE_DATA_SIZE MAX_PASSWORD_CHAR
//-----------------------------------------------------------------
typedef enum
{
	LCD_TASK_INIT_STATE=0,
	LCD_TASK_WELCOME_STATE,
	LCD_TASK_ENTER_PASSWORD_STATE,
	LCD_TASK_CHECK_PASSWORD_STATE,
	LCD_TASK_LOCKED_STATE
} lcd_task_state_t;
//----------------------------------------------------------
void vlcd_task(void *pvParameters);
//-----------------------------------------------------
xQueueHandle xlcd_task_q_get(void);
void vlcd_task_q_set(xQueueHandle key);
void vlcd_task_resume_semaphore_set(xSemaphoreHandle key);
//-----------------------------------------------------
xSemaphoreHandle xlcd_task_resume_semaphore_get(void);
lcd_task_state_t xlcd_task_get_state(void);
//----------------------------------------------------------
#endif /* LCD_TASK_H_ */