/*
 * motor_driver.c
 *
 * Created: 02/06/2018 14:23:40
 *  Author: BADROUS
 */ 
#include "motor_driver.h"


void vMotor_driver_init(motor_param_t* param)
{
	gpio_init_param_t temp;
	//init enable 1
	temp.direction=GPIO_OUTPUT;
	temp.output_init_value=STD_LOGIC_LOW;
	temp.port=param->enable1.port;
	temp.pin=param->enable1.pin;
	vGpio_Init(&temp);
	//init enable 2
	temp.direction=GPIO_OUTPUT;
	temp.output_init_value=STD_LOGIC_LOW;
	temp.port=param->enable2.port;
	temp.pin=param->enable2.pin;
	vGpio_Init(&temp);
	//init pwm pin
	temp.direction=GPIO_OUTPUT;
	temp.output_init_value=STD_LOGIC_LOW;
	temp.port=param->pwm_pin.port;
	temp.pin=param->pwm_pin.pin;
	vGpio_Init(&temp);
	//init pwm signal
	vPwm2_Init(param->pwm_param);
}
//--------------------------------------------------------------------
void vMotor_driver_rotate(motor_param_t* param,u8 power,motor_direction_type direction)
{
	gpio_write_param_t temp1,temp2;
	temp1.port=param->enable1.port;
	temp1.pin=param->enable1.pin;
	temp2.port=param->enable2.port;
	temp2.pin=param->enable2.pin;
	switch(direction)
	{
		
		case MOTOR_STOPED :			
									temp1.output=STD_LOGIC_LOW;
									temp2.output=STD_LOGIC_LOW;
									vGpio_Write(&temp1);
									vGpio_Write(&temp2);
									vPwm2_stop();
									break;
		case MOTOR_ROTATE_CLOCKWISE :
									temp1.output=STD_LOGIC_HIGH;
									temp2.output=STD_LOGIC_LOW;
									vGpio_Write(&temp1);
									vGpio_Write(&temp2);
									vPwm2_restart();
									vPwm2_set_duty(power);
									break;									
		case MOTOR_ROTATE_COUNTER_CLOCKWISE :
									temp1.output=STD_LOGIC_LOW;
									temp2.output=STD_LOGIC_HIGH;
									vGpio_Write(&temp1);
									vGpio_Write(&temp2);
									vPwm2_restart();
									vPwm2_set_duty(power);
									break;		
		
		default:break;
	}
}