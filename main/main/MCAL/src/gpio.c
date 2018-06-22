/*
 * gpio.c
 *
 * Created: 17/05/2018 13:42:28
 *  Author: BADROUS
 */ 
//----------------------------------------------------------------------
#include "gpio.h"
//--------------------------------------------------------------------
void vGpio_Init(gpio_init_param_t * gpio_param)
{
	//check port
	switch(gpio_param->port)
	{
	case GPIO_PORT1 :
					DDRA = (DDRA & ~(1<<gpio_param->pin)) | (((gpio_param->direction)& (0x01))<<gpio_param->pin);
					PORTA = (PORTA & ~(1<<gpio_param->pin)) | ((((gpio_param->direction)& (0x02))>>1)<<gpio_param->pin) | (gpio_param->output_init_value<<gpio_param->pin);
					break;
	case GPIO_PORT2 :
					DDRB = (DDRB & ~(1<<gpio_param->pin)) | (((gpio_param->direction)& (0x01))<<gpio_param->pin);
					PORTB = (PORTB & ~(1<<gpio_param->pin)) | ((((gpio_param->direction)& (0x02))>>1)<<gpio_param->pin) | (gpio_param->output_init_value<<gpio_param->pin);	
					break;					
	case GPIO_PORT3 :
					DDRC = (DDRC & ~(1<<gpio_param->pin)) | (((gpio_param->direction)& (0x01))<<gpio_param->pin);
					PORTC = (PORTC & ~(1<<gpio_param->pin)) | ((((gpio_param->direction)& (0x02))>>1)<<gpio_param->pin) | (gpio_param->output_init_value<<gpio_param->pin);	
					break;	
	case GPIO_PORT4 :
					DDRD = (DDRD & ~(1<<gpio_param->pin)) | (((gpio_param->direction)& (0x01))<<gpio_param->pin);
					PORTD = (PORTD & ~(1<<gpio_param->pin)) | ((((gpio_param->direction)& (0x02))>>1)<<gpio_param->pin) | (gpio_param->output_init_value<<gpio_param->pin);	
					break;
	default:break;
	}
}
//-------------------------------------------------------------------
void vGpio_Write(gpio_write_param_t * gpio_param)
{
		//check port
		switch(gpio_param->port)
		{
			case GPIO_PORT1 :
							PORTA = (PORTA & ~(1<<gpio_param->pin)) | (gpio_param->output<<gpio_param->pin);
							break;
			case GPIO_PORT2 :
							PORTB = (PORTB & ~(1<<gpio_param->pin)) |  (gpio_param->output<<gpio_param->pin);
							break;
			case GPIO_PORT3 :
							PORTC = (PORTC & ~(1<<gpio_param->pin)) | (gpio_param->output<<gpio_param->pin);
							break;
			case GPIO_PORT4 :
							PORTD = (PORTD & ~(1<<gpio_param->pin)) |  (gpio_param->output<<gpio_param->pin);
							break;
			default:break;
		}
}
//------------------------------------------------------------------
std_logic_t Gpio_Read(gpio_read_param_t * gpio_param)
{
	std_logic_t result=STD_LOGIC_LOW;
			//check port
			switch(gpio_param->port)
			{
				case GPIO_PORT1 :
								result=(PINA & (1<<(gpio_param->pin)))>>(gpio_param->pin);
								break;
				case GPIO_PORT2 :
								result=(PINB & (1<<(gpio_param->pin)))>>(gpio_param->pin);
								break;
				case GPIO_PORT3 :
								result=(PINC & (1<<(gpio_param->pin)))>>(gpio_param->pin);
								break;
				case GPIO_PORT4 :
								result=(PIND & (1<<(gpio_param->pin)))>>(gpio_param->pin);
								break;
				default:break;
			}
	return result;
}
//-------------------------------------------------------------------
