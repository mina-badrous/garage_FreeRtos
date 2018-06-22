/*
 * ecu_buzzer.c
 *
 * Created: 30/05/2018 23:00:57
 *  Author: BADROUS
 */
//-------------------------------------------------------------- 
#include "ecu_buzzer.h"
//---------------------------------------------------------------
void vEcu_Buzzer_init(ecu_buzzer_t * buzzer_param)
{
		gpio_init_param_t temp;
		temp.direction=GPIO_OUTPUT;
		temp.output_init_value=buzzer_param->output;
		temp.pin=buzzer_param->pin;
		temp.port=buzzer_param->port;
		vGpio_Init(&temp);
}
//----------------------------------------------------------------
void vEcu_Buzzer_alert(ecu_buzzer_t * buzzer_param)
{
		buzzer_param->output=STD_LOGIC_HIGH;
		vGpio_Write(buzzer_param);
}
//---------------------------------------------------------------
void vEcu_Buzzer_stop(ecu_buzzer_t * buzzer_param)
{
	buzzer_param->output=STD_LOGIC_LOW;
	vGpio_Write(buzzer_param);
}
//-------------------------------------------------------------
void vEcu_Buzzer_toggle(ecu_buzzer_t * buzzer_param)
{
	gpio_read_param_t temp;
	boolean val;
	temp.pin=buzzer_param->pin;
	temp.port=buzzer_param->port;
	val=Gpio_Read(&temp);
	buzzer_param->output= !(val);
	vGpio_Write(buzzer_param);
}
//----------------------------------------------------------------