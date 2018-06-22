/*
 * ecu_buzzer.h
 *
 * Created: 30/05/2018 23:00:46
 *  Author: BADROUS
 */ 


#ifndef ECU_BUZZER_H_
#define ECU_BUZZER_H_

#include "int.h"
#include "gpio.h"
typedef gpio_write_param_t ecu_buzzer_t;


void vEcu_Buzzer_init(ecu_buzzer_t * buzzer_param);
void vEcu_Buzzer_alert(ecu_buzzer_t * buzzer_param);
void vEcu_Buzzer_stop(ecu_buzzer_t * buzzer_param);
void vEcu_Buzzer_toggle(ecu_buzzer_t * buzzer_param);
#endif /* ECU_BUZZER_H_ */