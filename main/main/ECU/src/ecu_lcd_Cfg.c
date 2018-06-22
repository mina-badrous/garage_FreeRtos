/*
 * ecu_lcd_Cfg.c
 *
 * Created: 26/05/2018 18:55:32
 *  Author: BADROUS
 */ 
#include "ecu_lcd_Cfg.h"

//------------------------------------------------------------------
gpio_init_param_t lcd_d4 =
{
	GPIO_PORT1,				//port
	GPIO_PIN4,					//pin
	GPIO_OUTPUT,			//direction
	STD_LOGIC_LOW					//output_init_value
};
//--------------------------------------------------------------
gpio_init_param_t lcd_d5 =
{
	GPIO_PORT1,				//port
	GPIO_PIN5,					//pin
	GPIO_OUTPUT,			//direction
	STD_LOGIC_LOW					//output_init_value
};
//--------------------------------------------------------------
gpio_init_param_t lcd_d6 =
{
	GPIO_PORT1,				//port
	GPIO_PIN6,					//pin
	GPIO_OUTPUT,			//direction
	STD_LOGIC_LOW					//output_init_value
};
//--------------------------------------------------------------
gpio_init_param_t lcd_d7 =
{
	GPIO_PORT1,				//port
	GPIO_PIN7,					//pin
	GPIO_OUTPUT,			//direction
	STD_LOGIC_LOW					//output_init_value
};
//--------------------------------------------------------------
gpio_init_param_t lcd_rs =
{
	GPIO_PORT1,				//port
	GPIO_PIN1,					//pin
	GPIO_OUTPUT,			//direction
	STD_LOGIC_LOW					//output_init_value
};
//--------------------------------------------------------------
gpio_init_param_t lcd_e =
{
	GPIO_PORT1,				//port
	GPIO_PIN0,					//pin
	GPIO_OUTPUT,			//direction
	STD_LOGIC_LOW					//output_init_value
};
//--------------------------------------------------------------








//-----------------------------------------------------------------
ecu_lcd_init_param_t lcd_param =
{
	&lcd_d4,
	&lcd_d5,
	&lcd_d6,
	&lcd_d7,
	
	&lcd_rs,
	&lcd_e
};