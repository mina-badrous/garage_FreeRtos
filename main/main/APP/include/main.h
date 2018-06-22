/*
 * main.h
 *
 * Created: 30/05/2018 21:15:46
 *  Author: BADROUS
 */ 
//--------------------------------------------------------------
#ifndef MAIN_H_
#define MAIN_H_
//--------------------------------------------------------------avr
#include <avr/pgmspace.h>
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "int.h"
#include "bitwise.h"
//-----------------------------------------------------------MCAL
/*
#include "gpio.h"
#include "i2c_Cfg.h"
#include "pwm2_Cfg.h"
#include "uart_Cfg.h"
//-----------------------------------------------------------ECU
#include "ecu_buzzer_Cfg.h"
#include "ecu_keypad_Cfg.h"
//#include "ecu_lcd.h"
//#include "ecu_lcd_Cfg.h"
#include "ecu_led_Cfg.h"
*/
//-----------------------------------------------------------SERVICE
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
//-----------------------------------------------------------APP
#ifdef APP_HEADER
#include "keypad_task.h"
#include "led_task.h"
#include "log_task.h"
#include "motor_task.h"
#include "terminal_task.h"
#include "lcd_task.h"
#include "spi_interrupt.h"
#endif
//-----------------------------------------------------------main
extern SemaphoreHandle_t uart_mutex;
extern SemaphoreHandle_t i2c_mutex;
extern TaskHandle_t keypad_task_handler;
//-------------------------------------------------------------------
#endif /* MAIN_H_ */