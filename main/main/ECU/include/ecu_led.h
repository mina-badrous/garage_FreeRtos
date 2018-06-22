/*
 * ecu_led.h
 *
 * Created: 30/05/2018 23:00:46
 *  Author: BADROUS
 */ 


#ifndef ECU_LED_H_
#define ECU_LED_H_

#include "int.h"
#include "gpio.h"

typedef gpio_write_param_t ecu_led_t;

void vEcu_led_init(ecu_led_t * led_param);
void vEcu_set_led_on(ecu_led_t * led_param);
void vEcu_set_led_off(ecu_led_t * led_param);
void vEcu_toggle_led(ecu_led_t * led_param);

#endif /* ECU_LED_H_ */