/*
 * uart.h
 *
 * Created: 13/05/2018 18:26:30
 *  Author: BADROUS
 */ 


#ifndef UART_H_
#define UART_H_
//----------------------------------------------------------
#include "int.h"
#include "bitwise.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
//-------------------------------------------------------------------
#define MAX_UART_INTERRUPTS 3U
#define RX_COMPLETE_INTERRUPT 0U
#define TX_COMPLETE_INTERRUPT 1U
#define UDR_EMPTY_INTERRUPT 2U
//-----------------------------------------------------------
typedef enum
{
	UART_1_STOP_BIT=0,
	UART_2_STOP_BIT	
} uart_stop_bits;
//----------------------------------------------------------------
typedef enum
{
	UART_NON_PARITY=0,
	UART_PARIT_RESERVED	,
	UART_EVEN_PARITY,
	UART_ODD_PARITY
} uart_parity_mode;
//------------------------------------------------------------------
typedef enum
{
	UART_5_BITS=0,
	UART_6_BITS,
	UART_7_BITS,
	UART_8_BITS,
	UART_9_BITS
} uart_data_bits;
//-------------------------------------------------------------
typedef enum
{
	NORMAL_ASYNC_MODE=0,
	DOUBLE_SPEED_ASYNC_MODE
} uart_async_mode;
typedef enum
{
	UART_RECIEVER=0,
	UART_TRANSMITER,
	UART_RECIEVER_TRANSMITER
} uart_mode_t;
//------------------------------------------------------------------
#define UART_DISABLE_INTERRUPT 0
#define UART_ENABLE_RX_COMPLETE_INTERRUPT (1<<RXCIE)
#define UART_ENABLE_TX_COMPLETE_INTERRUPT (1<<TXCIE)
#define UART_ENABLE_UDR_EMPTY_INTERRUPT (1<<UDRIE)
//-------------------------------------------------------------------
typedef void (*PTR2ISR)(void);
//-------------------------------------------------------------------
typedef struct
{
	u32 baud_rate;
	u32 sys_clock;
	uart_mode_t   mode;
	uart_async_mode async_mode;
	uart_data_bits data_bits;
	uart_stop_bits stop_bits;
	uart_parity_mode parity_mode;
	u8 interrupt_mode;
	PTR2ISR rx_ISR;
	PTR2ISR tx_ISR;
	PTR2ISR data_reg_empty_ISR;
} uart_param_t;
//----------------------------------------FUNCTION PROTOTYPE
void vUartInit(uart_param_t * uart_param);
void vUartTransmit(u8 data);
void vUartTransmitStream(u8* data,u8 count);
void vUartTransmitString(char* data);

void vUartTransmitStream_program(const u8* data,u8 count);
void vUartTransmitString_program(const char* data);
u8 u8UartReceive();

//----------------------------------------------------------------
#endif /* UART_H_ */