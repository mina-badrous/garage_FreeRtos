/*
 * pwm2.h
 *
 * Created: 29/05/2018 12:50:41
 *  Author: BADROUS
 */ 


#ifndef PWM2_H_
#define PWM2_H_
//-------------------------------------------------------------
#include "int.h"
#include "bitwise.h"
#include "avr/io.h"
#include <stdlib.h>
#include <avr/interrupt.h>
//-------------------------------------------------------------------
#define PWM2_PRESCALER_MASK 0xF8
//-------------------------------------------------------------------
typedef enum
{
	PWM2_FAST_NON_INVERTING_MODE=0,
	PWM2_FAST_INVERTING_MODE,
	PWM2_PHASE_CORRECT_CLEAR_UP_SET_DOWN,
	PWM2_PHASE_CORRECT_SET_UP_CLEAR_DOWN
} pwm2_mode_t;
//---------------------------------------------------------
typedef enum
{
	PWM2_BY1_PRESCALER=0,
	PWM2_BY8_PRESCALER,
	PWM2_BY32_PRESCALER,
	PWM2_BY64_PRESCALER,
	PWM2_BY128_PRESCALER,
	PWM2_BY256_PRESCALER,
	PWM2_BY1024_PRESCALER
} pwm2_prescaler_t;

//------------------------------------------------------------------
typedef struct
{
	pwm2_mode_t			mode;
	pwm2_prescaler_t	prescaler;
	u8					duty_cycle;
} pwm2_init_param_t;

//------------------------------------------------------------------
void vPwm2_Init(pwm2_init_param_t * param);
void vPwm2_set_duty(u8 duty);
void vPwm2_stop(void);
void vPwm2_restart(void);
//----------------------------------------------------------------------
#endif /* PWM2_H_ */