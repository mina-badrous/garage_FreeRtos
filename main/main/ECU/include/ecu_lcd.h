/*
 * ecu_lcd.h
 *
 * Created: 26/05/2018 18:22:04
 *  Author: BADROUS
 */ 


#ifndef ECU_LCD_H_
#define ECU_LCD_H_
//--------------------------------------------------------------

#if 0
#include "int.h"
#include "bitwise.h"
#define F_CPU 1000000UL
#include <util/delay.h>
#include <avr/io.h>
#endif

#include "main.h"
#include "gpio.h"
#include <stdlib.h>
#include <string.h>
//--------------------------------------------------------------
//----------------------------------------------------------
#define CMD 0
#define DATA 1
//-------------------------------------------------------CMDs
#define LCD_CMD_MODE4_INIT0 0x33
#define LCD_CMD_MODE4_INIT1 0x32
#define LCD_CMD_MODE4_2LINES 0x28
//-------------------------------
#define LCD_CMD_DISPLAY_ON_CURSOR_OFF 0x0c
#define LCD_CMD_DISPLAY_OFF_CURSOR_OFF 0x08
#define LCD_CMD_DISPLAY_ON_CURSOR_ON_NO_BLINK 0x0E
#define LCD_CMD_DISPLAY_ON_CURSOR_ON_WITH_BLINK 0x0F
//---------------------------------------------
#define LCD_CMD_CLEAR_DISPLAY 0x01
#define LCD_CMD_RETURN_HOME 0x02
//--------------------------------------------- set and not move
#define LCD_CMD_SET_CURSOR_MOVE_DIRECTION_RIGHT 0x06
#define LCD_CMD_SET_CURSOR_MOVE_DIRECTION_LEFT 0x04
//--------------------------------------------moving cursor
#define LCD_CMD_CURSOR_MOVE_RIGHT 0x14
#define LCD_CMD_CURSOR_MOVE_LEFT 0x10
//--------------------------------------
#define LCD_CMD_DISPLAY_SHIFT_RIGHT 0x18  /*must be shifted left first*/
#define LCD_CMD_DISPLAY_SHIFT_LEFT 0x1c
//----------------------------------
//end CMDs
//-------------------------------------
#define LCD_FIRST_LINE 0
#define LCD_SECOND_LINE 1
#define FIRST_LINE_ADDRESS 0x80
#define SECOND_LINE_ADDRESS 0xC0
//-------------------------------------
#define LCD_NO_ERROR 0
#define LCD_NO_CHAR_TO_PRINT 1
#define LCD_ERROR_ROW_OUT_OF_RANGE 2
#define LCD_ERROR_COLUMN_OUT_OF_RANGE 3
//----------------------------------------------------------
typedef struct
{
	gpio_init_param_t* d4;
	gpio_init_param_t* d5;
	gpio_init_param_t* d6;
	gpio_init_param_t* d7;
	
	gpio_init_param_t* rs;
	gpio_init_param_t* e;
	
} ecu_lcd_init_param_t;
//----------------------------------------------------interface
void vEcu_LcdInit(ecu_lcd_init_param_t* param);
void vEcu_LcdSendChar(char data);
u8 u8Ecu_LcdSendString(char* str_ptr);
void vEcu_LcdSendCmd(char data);
void vEcu_LcdClearDisplay(void);
u8 u8Ecu_LcdGoToXy(u8 line,u8 column);
//-----------------------------------------------high level interface
u8 u8Ecu_LcdPrint(u8 data,u8 line,u8 position);
//----------------------------------------------------------------
void vEcu_LcdSendPassword(char data);
void vEcu_LcdRemovePassword(void);



#endif /* ECU_LCD_H_ */