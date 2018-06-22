/*
 * motor_driver.h
 *
 * Created: 02/06/2018 14:24:00
 *  Author: BADROUS
 */ 


#ifndef MOTOR_DRIVER_H_
#define MOTOR_DRIVER_H_

#include "main.h"
#include "gpio.h"
#include "pwm2_Cfg.h"

typedef gpio_write_param_t motor_driver_enable_t;
typedef gpio_write_param_t motor_driver_pwm_pin_t;
typedef pwm2_init_param_t motor_driver_pwm_param_t;

typedef enum
{
	MOTOR_STOPED=0,
	MOTOR_ROTATE_CLOCKWISE,
	MOTOR_ROTATE_COUNTER_CLOCKWISE
} motor_direction_type;

typedef struct
{
	motor_driver_enable_t enable1;
	motor_driver_enable_t enable2;
	motor_driver_pwm_pin_t pwm_pin;
	motor_driver_pwm_param_t*  pwm_param; 
} motor_param_t;

void vMotor_driver_init(motor_param_t* param);
void vMotor_driver_rotate(motor_param_t* param,u8 power,motor_direction_type direction);	

#endif /* MOTOR_DRIVER_H_ */