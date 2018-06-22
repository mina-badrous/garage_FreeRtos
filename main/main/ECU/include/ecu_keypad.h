/*
 * ecu_keypad.h
 *
 * Created: 24/05/2018 15:42:56
 *  Author: BADROUS
 */ 


#ifndef ECU_KEYPAD_H_
#define ECU_KEYPAD_H_
//===============================================================
#include "int.h"
#include "gpio.h"
//-------------------------------------------------------
#define NO_VALID_INPUT		0xFF
//-----------------------------------------------------
//--------------------------------------------------------
#define IS_VALID			0


//------------------------------------------------------------------
typedef enum
{
	KP_SWITCH_RELEASED_STATE=0,
	KP_SWITCH_PRE_PRESSED_STATE,
	KP_SWITCH_PRESSED_STATE,
	KP_SWITCH_PRE_RELEASED_STATE,
	
} keypad_switch_state_t;
//------------------------------------------------------------
typedef struct 
{
	gpio_init_param_t* row0;      //output
	gpio_init_param_t* row1;		 //output
	gpio_init_param_t* row2;		 //output
	gpio_init_param_t* row3;		 //output
	
	gpio_init_param_t* col0;		//input 
	gpio_init_param_t* col1;		//input
	gpio_init_param_t* col2;		//input
	gpio_init_param_t* col3;		//input
	
} ecu_keypad_init_param_t;
//------------------------------------------------------------
void vEcu_KeypadInit(ecu_keypad_init_param_t* param);
unsigned char ucEcu_KeypadScan(ecu_keypad_init_param_t* param);
keypad_switch_state_t ucEcu_KeypadScanState(ecu_keypad_init_param_t* param,unsigned char* button);
//---------------------------------------------------------------
#endif /* ECU_KEYPAD_H_ */