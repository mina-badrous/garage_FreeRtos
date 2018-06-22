/*
 * uart.c
 *
 * Created: 13/05/2018 18:26:50
 *  Author: BADROUS
 */ 
#include "uart.h"

static PTR2ISR g_ISR[MAX_UART_INTERRUPTS];

void vUartInit(uart_param_t * uart_param)
{
	u16 ubr=0;
	u8 ucsrc_buffer=0;
			//calculate UBRR
			
	



	//set URSEL
	//set_bit(UCSRC,URSEL);
	//UCSRC =0x80;
	//select mode
	switch(uart_param->mode)
	{
		case UART_RECIEVER :set_bit(UCSRB,RXEN);clear_bit(UCSRB,TXEN);break;
		case UART_TRANSMITER :clear_bit(UCSRB,RXEN);set_bit(UCSRB,TXEN);break;
		case UART_RECIEVER_TRANSMITER :set_bit(UCSRB,RXEN);set_bit(UCSRB,TXEN);break;
		default:break;
	}
	
	//set data bits
	switch(uart_param->data_bits)
	{
		case UART_5_BITS :clear_bit(UCSRB,UCSZ2);clear_bit(ucsrc_buffer,UCSZ1);clear_bit(ucsrc_buffer,UCSZ0);break;
		case UART_6_BITS :clear_bit(UCSRB,UCSZ2);clear_bit(ucsrc_buffer,UCSZ1);set_bit(ucsrc_buffer,UCSZ0);break;
		case UART_7_BITS :clear_bit(UCSRB,UCSZ2);set_bit(ucsrc_buffer,UCSZ1);clear_bit(ucsrc_buffer,UCSZ0);break;
		case UART_8_BITS :clear_bit(UCSRB,UCSZ2);set_bit(ucsrc_buffer,UCSZ1);set_bit(ucsrc_buffer,UCSZ0);break;
		case UART_9_BITS :set_bit(UCSRB,UCSZ2);set_bit(ucsrc_buffer,UCSZ1);set_bit(ucsrc_buffer,UCSZ0);break;
		default:break;
	}
	//set stop bits
	switch(uart_param->stop_bits)
	{
		case UART_1_STOP_BIT :clear_bit(ucsrc_buffer,USBS);break;
		case UART_2_STOP_BIT :set_bit(ucsrc_buffer,USBS);break;
		default:break;
	}
	//set parity mode
	switch(uart_param->parity_mode)
	{
		case UART_NON_PARITY :clear_bit(ucsrc_buffer,UPM1);clear_bit(ucsrc_buffer,UPM0);break;
		case UART_EVEN_PARITY :set_bit(ucsrc_buffer,UPM1);clear_bit(ucsrc_buffer,UPM0);break;
		case UART_ODD_PARITY :set_bit(ucsrc_buffer,UPM1);set_bit(ucsrc_buffer,UPM0);break;
		default:break;
	}
	// write UCSRC data
	set_bit(ucsrc_buffer,URSEL);
	set_bit(UCSRC,URSEL);
	UCSRC =ucsrc_buffer |(1<<URSEL);
	//set interrupts
	UCSRB |=uart_param->interrupt_mode;
	if(uart_param->rx_ISR !=NULL)
	{
		g_ISR[RX_COMPLETE_INTERRUPT]=uart_param->rx_ISR;
	}
	if(uart_param->tx_ISR !=NULL)
	{
		g_ISR[TX_COMPLETE_INTERRUPT]=uart_param->tx_ISR;
	}
	if(uart_param->data_reg_empty_ISR !=NULL)
	{
		g_ISR[UDR_EMPTY_INTERRUPT]=uart_param->data_reg_empty_ISR;
	}	
	
	//-------------------------------------------------
		//select UBRR reg
		//clear_bit(UCSRC,URSEL);
		//set async mode
		//clear_bit(UCSRC,UMSEL);
		switch(uart_param->async_mode)
		{
			case NORMAL_ASYNC_MODE :clear_bit(UCSRA,U2X);
			ubr=(u16)((uart_param->sys_clock)/((u32)16 *(uart_param->baud_rate)))-1;
			UBRRH=(u8)((u16)ubr>>8) & (0x7F);
			UBRRL=(u8)ubr;
			break;
			case DOUBLE_SPEED_ASYNC_MODE :set_bit(UCSRA,U2X);
			ubr=(u16)((uart_param->sys_clock)/((u32)8 *(uart_param->baud_rate)))-1;
			UBRRH=(u8)((u16)ubr>>8)  & (0x7F);
			UBRRL=(u8)ubr;
			break;
			default:break;
		}
	
}
//-------------------------------------------------------------------------
ISR(USART_RXC_vect)
{
	if(g_ISR[RX_COMPLETE_INTERRUPT] !=NULL)
	{
		g_ISR[RX_COMPLETE_INTERRUPT]();
	}
}
//------------------------------------------------------------
ISR(USART_TXC_vect)
{
	if(g_ISR[TX_COMPLETE_INTERRUPT] !=NULL)
	{
		g_ISR[TX_COMPLETE_INTERRUPT]();
	}
}
//-------------------------------------------------------
ISR(USART_UDRE_vect)
{
	if(g_ISR[UDR_EMPTY_INTERRUPT] !=NULL)
	{
		g_ISR[UDR_EMPTY_INTERRUPT]();
	}
}
//--------------------------------------------------
void vUartTransmit(u8 data)
{
	while(check_bit(UCSRA,UDRE) == 0);
	UDR=data;	
}
//---------------------------------------------------------
u8 u8UartReceive()
{
	while(check_bit(UCSRA,RXC) ==0);
	return UDR;
}
//--------------------------------------------------------
void vUartTransmitStream(u8* data,u8 count)
{
	u8 index=0;
	for(index=0;index<count;index++)
	{
		vUartTransmit(*(data++));
	}
}
//----------------------------------------------
void vUartTransmitString(char* data)
{
		while(*data != '\0')
		{
			vUartTransmit(*(data++));
		}
}
//-----------------------------------------------------
void vUartTransmitStream_program(const u8* data,u8 count)
{
		u8 index=0;
		for(index=0;index<count;index++)
		{
			vUartTransmit(pgm_read_byte(data++));
		}
}
void vUartTransmitString_program(const char* data)
{
		while(pgm_read_byte(data) != '\0')
		{
			vUartTransmit(pgm_read_byte(data++));
		}	
}