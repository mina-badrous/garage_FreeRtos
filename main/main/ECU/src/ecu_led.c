/*
 * ecu_led.c
 *
 * Created: 30/05/2018 23:00:57
 *  Author: BADROUS
 */
//-------------------------------------------------------------- 
#include "ecu_led.h"
//---------------------------------------------------------------
//---------------------------------------------------------------
void vEcu_led_init(ecu_led_t * led_param)
{
	gpio_init_param_t temp;
	temp.direction=GPIO_OUTPUT;
	temp.output_init_value=led_param->output;
	temp.pin=led_param->pin;
	temp.port=led_param->port;
	vGpio_Init(&temp);
}
//-----------------------------------------------------------------
void vEcu_set_led_on(ecu_led_t * led_param)
{
	led_param->output=STD_LOGIC_HIGH;
	vGpio_Write(led_param);
}
//----------------------------------------------------------------
void vEcu_set_led_off(ecu_led_t * led_param)
{
	led_param->output=STD_LOGIC_LOW;
	vGpio_Write(led_param);
}
//----------------------------------------------------------------
void vEcu_toggle_led(ecu_led_t * led_param)
{
	gpio_read_param_t temp;
	boolean val;
	temp.pin=led_param->pin;
	temp.port=led_param->port;
	val=Gpio_Read(&temp);
	led_param->output= !(val);
	vGpio_Write(led_param);
}
//----------------------------------------------------------------

//----------------------------------------------------------------