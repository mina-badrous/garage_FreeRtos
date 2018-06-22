/*
 * ecu_keypad_Cfg.c
 *
 * Created: 24/05/2018 15:43:52
 *  Author: BADROUS
 */ 
#include "ecu_keypad_Cfg.h"
//---------------------------------------------------------------------
gpio_init_param_t row0 = 
{
	GPIO_PORT2,				//port
	GPIO_PIN0,					//pin
	GPIO_OUTPUT,			//direction
	STD_LOGIC_HIGH					//output_init_value
};
//---------------------------------------------------------------------
gpio_init_param_t row1 =
{
	GPIO_PORT2,				//port
	GPIO_PIN1,					//pin
	GPIO_OUTPUT,			//direction
	STD_LOGIC_HIGH					//output_init_value
};
//--------------------------------------------------------------------
gpio_init_param_t row2 =
{
	GPIO_PORT2,				//port
	GPIO_PIN2,					//pin
	GPIO_OUTPUT,			//direction
	STD_LOGIC_HIGH					//output_init_value
};
//----------------------------------------------------------------------
gpio_init_param_t row3 =
{
	GPIO_PORT2,				//port
	GPIO_PIN3,					//pin
	GPIO_OUTPUT,			//direction
	STD_LOGIC_HIGH					//output_init_value
};
//-------------------------------------------------------------------
gpio_init_param_t col0 =
{
	GPIO_PORT3,				//port
	GPIO_PIN2,					//pin
	GPIO_INPUT_PULLUP,			//direction
	STD_LOGIC_LOW					//output_init_value
};
//-------------------------------------------------------------------
gpio_init_param_t col1 =
{
	GPIO_PORT3,				//port
	GPIO_PIN3,					//pin
	GPIO_INPUT_PULLUP,			//direction
	STD_LOGIC_LOW					//output_init_value
};
//------------------------------------------------------------------
gpio_init_param_t col2 =
{
	GPIO_PORT3,				//port
	GPIO_PIN4,					//pin
	GPIO_INPUT_PULLUP,			//direction
	STD_LOGIC_LOW				//output_init_value
};
//-----------------------------------------------------------------

gpio_init_param_t col3 =
{
	GPIO_PORT4,				//port
	GPIO_PIN3,					//pin
	GPIO_INPUT_PULLUP,			//direction
	STD_LOGIC_LOW					//output_init_value
};
//----------------------------------------------------------------
ecu_keypad_init_param_t keypad_param =
{
	&row0,
	&row1,
	&row2,
	&row3,
	&col0,
	&col1,
	&col2,
	&col3
};