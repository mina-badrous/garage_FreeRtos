/*
 * pwm2_Cfg.c
 *
 * Created: 29/05/2018 14:19:44
 *  Author: BADROUS
 */ 
#include "pwm2_Cfg.h"


pwm2_init_param_t pwm2_param =
{
		PWM2_PHASE_CORRECT_CLEAR_UP_SET_DOWN,			//mode
		PWM2_BY1024_PRESCALER,					//prescaler
		0									//duty_cycle
};