/*
 * terminal_task.h
 *
 * Created: 02/06/2018 16:18:42
 *  Author: BADROUS
 */ 
//-----------------------------------------------------------------
#ifndef TERMINAL_TASK_H_
#define TERMINAL_TASK_H_
//------------------------------------------------------------------
#define APP_HEADER
#include "main.h"
#include "terminal_main_menu.h"
#include "terminal_user_menu.h"

#include "uart_Cfg.h"
#include "rtc.h"
#include "external_eeprom.h"
#include "lcd_task.h"
//------------------------------------------------------------------
#define FAILED_TO_SET_TIME 1
#define SUCCESS_TO_SET_TIME 0
#define INVALID_INPUT 0xFF
#define VALID_NAME 0

#define PASSWORD_NOT_VALID 0xff
#define PASSWORD_VALID 0

#define USERS_MENU 4U
#define LOG_OUT 5
#define BACK_TO_MAIN_MENU 5

#define MAX_TERMINAL_BUFFER_DATA 10
#define MAX_MENU_STRING MAX_TERMINAL_BUFFER_DATA

#define TERMINAL_TASK_QUEUE_SIZE MAX_TERMINAL_BUFFER_DATA
#define TERMINAL_TASK_QUEUE_DATA_SIZE 1
#define ENTER_KEY 0x0D
#define BACKSPACE_KEY 0x08
#define AT_KEY '@'
#define TERMINAL_TIME_OUT 0x55
#define TERMINAL_RETURN_U8 0xAA
#define COMMAND_NOT_FULL 1U
#define COMMAND_FULL 0
#define MAX_MENU_OPTIONS 6
#define MAX_USERS_MENU_OPTIONS 6
//-------------------------------------------------------------------
typedef enum
{
	TERMINAL_TASK_FACTORY_STATE=0,
	TERMINAL_TASK_LOGGED_OFF_STATE,
	TERMINAL_TASK_CHECK_ADMIN_PASS_STATE,
	TERMINAL_TASK_MENU_STATE,
	TERMINAL_TASK_USERS_MENU_STATE,
	TERMINAL_TASK_LOCKED_STATE
} terminal_task_state_t;
//----------------------------------------------------------------
void vTerminal_task(void *pvParameters);
//---------------------------------------------------------------
u8 get_u8(u8* data);
void rx_complete_isr_func(void);
//-------------------------------------------------------
xQueueHandle xTerminal_task_q_get(void);
void vTerminal_task_q_set(xQueueHandle key);
//---------------------------------------------------------
u8 get_user_name(u8* data);
u8 u8Get_user_password_arr(u8 * password);
u8 u8Get_terminal_task_state(void);
void vSet_terminal_task_state(u8 state);
//--------------------------------------------------------------
#endif /* TERMINAL_TASK_H_ */