/*
 * ecu_led_Cfg.c
 *
 * Created: 30/05/2018 23:01:26
 *  Author: BADROUS
 */ 
#include "ecu_led_Cfg.h"


ecu_led_t green_led_param =
{
		GPIO_PORT3,
		GPIO_PIN7,
		STD_LOGIC_LOW
};
//-------------------------------------------------------
ecu_led_t red_led_param =
{
		GPIO_PORT3,
		GPIO_PIN6,
		STD_LOGIC_LOW
};
