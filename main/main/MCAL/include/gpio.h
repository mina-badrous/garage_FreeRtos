/*
 * gpio.h
 *
 * Created: 17/05/2018 13:42:11
 *  Author: BADROUS
 */ 
//----------------------------------------------------------------
#ifndef GPIO_H_
#define GPIO_H_
//--------------------------------------------------------------
#include "avr/io.h"
#include "int.h"
#include "bitwise.h"
//-----------------------------------------------------------------------
typedef enum
{
	STD_LOGIC_LOW=0,
	STD_LOGIC_HIGH
} std_logic_t;
//----------------------------------------------------------------------
typedef enum
{
	GPIO_PORT1=0,
	GPIO_PORT2,
	GPIO_PORT3,
	GPIO_PORT4	
} gpio_port_t;
//---------------------------------------------------------------------
typedef enum
{
	GPIO_PIN0=0,
	GPIO_PIN1,
	GPIO_PIN2,
	GPIO_PIN3,
	GPIO_PIN4,
	GPIO_PIN5,
	GPIO_PIN6,
	GPIO_PIN7
} gpio_pin_t;
//------------------------------------------------------------------------
typedef enum
{
	GPIO_INPUT=0,
	GPIO_OUTPUT,
	GPIO_INPUT_PULLUP
} gpio_direction_t;
//----------------------------------------------------------------------
typedef struct
{
	gpio_port_t				port;
	gpio_pin_t				pin;
	gpio_direction_t		direction;
	std_logic_t				output_init_value;
} gpio_init_param_t;
//----------------------------------------------------------------------
typedef struct
{
	gpio_port_t				port;
	gpio_pin_t				pin;
	std_logic_t				output;
} gpio_write_param_t;
//---------------------------------------------------------------------
typedef struct
{
	gpio_port_t				port;
	gpio_pin_t				pin;
} gpio_read_param_t;
//----------------------------------------------------function prototype
void vGpio_Init(gpio_init_param_t * gpio_param);
void vGpio_Write(gpio_write_param_t * gpio_param);
std_logic_t Gpio_Read(gpio_read_param_t * gpio_param);
//---------------------------------------------------------------------

#endif /* GPIO_H_ */