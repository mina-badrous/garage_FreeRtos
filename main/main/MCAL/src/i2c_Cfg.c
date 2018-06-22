/*
 * i2c_Cfg.c
 *
 * Created: 20/05/2018 10:53:51
 *  Author: BADROUS
 */ 
//---------------------------------------------------------
#include "i2c_Cfg.h"
//--------------------------------------------------------

const i2c_init_param_t i2c_param=
{
	16000000UL,                //F_CPU
	100000UL,					//desired_freq
	I2C_PRESCALER_BY_1,          //prescaler
	I2C_DISABLE_INTERRUPT,      //interrupt state
	0,							//slave address 
	NULL						//callback func
};