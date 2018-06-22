/*
 * motor_driver_Cfg.c
 *
 * Created: 02/06/2018 14:32:57
 *  Author: BADROUS
 */ 
#include "motor_driver_Cfg.h"



motor_param_t motor_param=
{
	{
		GPIO_PORT4,
		GPIO_PIN5,
		STD_LOGIC_LOW
	},
	{
		GPIO_PORT4,
		GPIO_PIN6,
		STD_LOGIC_LOW		
	},
	{
		GPIO_PORT4,
		GPIO_PIN7,
		STD_LOGIC_LOW
	},	
	&pwm2_param
};