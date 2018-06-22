/*
 * pwm2.c
 *
 * Created: 29/05/2018 12:50:49
 *  Author: BADROUS
 */ 
#include "pwm2.h"

static void set_prescaler2(pwm2_prescaler_t prescaler);
static pwm2_prescaler_t g_prescaler2;

//--------------------------------------------------------------
void vPwm2_Init(pwm2_init_param_t * param)
{ 
	//stop timer 0
	TCCR2 &=PWM2_PRESCALER_MASK;
	//set duty
	vPwm2_set_duty(param->duty_cycle);
	//set prescaler
	g_prescaler2=param->prescaler;
	set_prescaler2(g_prescaler2);
	switch(param->mode)
	{
		case PWM2_FAST_NON_INVERTING_MODE :
										//set mode 
										set_bit(TCCR2,WGM21);set_bit(TCCR2,WGM20);
										//set oc0 pin 
										set_bit(TCCR2,COM21);clear_bit(TCCR2,COM20);
										break;
		case PWM2_FAST_INVERTING_MODE :
										//set mode
										set_bit(TCCR2,WGM21);set_bit(TCCR2,WGM20);
										//set oc0 pin
										set_bit(TCCR2,COM21);set_bit(TCCR2,COM20);
										break;		
		case PWM2_PHASE_CORRECT_CLEAR_UP_SET_DOWN:
										//set mode
										clear_bit(TCCR2,WGM21);set_bit(TCCR2,WGM20);
										//set oc0 pin
										set_bit(TCCR2,COM21);clear_bit(TCCR2,COM20);
										break;		
		case PWM2_PHASE_CORRECT_SET_UP_CLEAR_DOWN :
										//set mode
										clear_bit(TCCR2,WGM21);set_bit(TCCR2,WGM20);
										//set oc0 pin
										set_bit(TCCR2,COM21);set_bit(TCCR2,COM20);
										break;		
		default:break;
	}
}
//--------------------------------------------------------------

static void set_prescaler2(pwm2_prescaler_t prescaler)
{
	switch(prescaler)
	{
		case PWM2_BY1_PRESCALER      :clear_bit(TCCR2,CS22);clear_bit(TCCR2,CS21);set_bit(TCCR2,CS20);break;
		case PWM2_BY8_PRESCALER      :clear_bit(TCCR2,CS22);set_bit(TCCR2,CS21)  ;clear_bit(TCCR2,CS20);break;
		case PWM2_BY32_PRESCALER     :clear_bit(TCCR2,CS22);set_bit(TCCR2,CS21);set_bit(TCCR2,CS20);break;
		case PWM2_BY64_PRESCALER     :set_bit(TCCR2,CS22);clear_bit(TCCR2,CS21);clear_bit(TCCR2,CS20);break;
		case PWM2_BY128_PRESCALER    :set_bit(TCCR2,CS22);clear_bit(TCCR2,CS21);set_bit(TCCR2,CS20);break;
		case PWM2_BY256_PRESCALER    :set_bit(TCCR2,CS22);set_bit(TCCR2,CS21);clear_bit(TCCR2,CS20);break;
		case PWM2_BY1024_PRESCALER   :set_bit(TCCR2,CS22);set_bit(TCCR2,CS21);set_bit(TCCR2,CS20);break;
		default						 :clear_bit(TCCR2,CS22);clear_bit(TCCR2,CS21);clear_bit(TCCR2,CS20);break;
	}
}
//----------------------------------------------------------
void vPwm2_set_duty(u8 duty)
{
	OCR2=(u8)(((u16)duty  * 255)/100);
}
//--------------------------------------------------------------
void vPwm2_stop(void)
{
	TCCR2 &=PWM2_PRESCALER_MASK;
}
//---------------------------------------------------------------
void vPwm2_restart(void)
{
	set_prescaler2(g_prescaler2);
}