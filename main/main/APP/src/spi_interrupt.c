/*
 * spi_interrupt.c
 *
 * Created: 19/06/2018 17:58:12
 *  Author: BADROUS
 */ 
#include "spi_interrupt.h"

const static u8 spi_rx_default_pass[4]={'m','i','n','a'};
static u8 spi_rx_buffer[4];
static u8 spi_rx_buffer_index=0;
volatile u8 g_unlock_system_flag=0;
void spi_interrupt(void)
{
	u8 error=0;
	//save coming byte
	spi_rx_buffer[spi_rx_buffer_index]=SPDR;
	//increment index and check limit
	spi_rx_buffer_index++;
	if(spi_rx_buffer_index==4)
	{
		
		//check buffer
		for(spi_rx_buffer_index=0;spi_rx_buffer_index<4;spi_rx_buffer_index++)
		{
			if(spi_rx_buffer[spi_rx_buffer_index] != spi_rx_default_pass[spi_rx_buffer_index])
			{
				error++;
			}
		} 
		if(error ==0)
		{
			g_unlock_system_flag=1;
		}
		spi_rx_buffer_index=0;
	}
}