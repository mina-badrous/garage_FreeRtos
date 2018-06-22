/*
 * uart_Cfg.c
 *
 * Created: 13/05/2018 18:27:28
 *  Author: BADROUS
 */ 
#include "uart_Cfg.h"

uart_param_t uart0={9600U,    //baud rate
					16000000UL,   //system clock
					UART_RECIEVER_TRANSMITER,  // mode of operation 
					NORMAL_ASYNC_MODE,  //async mode type 
					UART_8_BITS,       //data bits
					UART_1_STOP_BIT,     //stop bits
					UART_NON_PARITY	,	//parity bits	
					UART_ENABLE_RX_COMPLETE_INTERRUPT,	//interrupt mode
					NULL,					//rx ISR
					NULL,					//TX ISR
					NULL					// UDR ISR	
					};