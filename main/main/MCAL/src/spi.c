/*
 * spi.c
 *
 * Created: 17/06/2018 2:28:23
 *  Author: BADROUS
 */ 
#include "spi.h"

static ptr2Spi g_func_ptr=NULL;
//-----------------------------------------------------------------
void vSpi_init(spi_init_param_t * param)
{
	//set mode 
	switch(param->mode)
	{
		case SPI_SLAVE :
						clear_bit(SPCR,MSTR);
						//make ss MOSI and sck input and MISO output low
						clear_bit(DDRB,4);
						clear_bit(DDRB,5);
						clear_bit(DDRB,7);
						set_bit(DDRB,6);
						clear_bit(PORTB,6);
						break;
		case SPI_MASTER :
						set_bit(SPCR,MSTR);
						//make ss MOSI and sck output and MISO input 
						set_bit(DDRB,4);
						set_bit(DDRB,5);
						set_bit(DDRB,7);
						clear_bit(DDRB,6);
						break;
		default:break;
	}
	//set idle state
	switch(param->idle)
	{
		case SPI_CLK_IDLE_LOW :clear_bit(SPCR,CPOL);break;
		case SPI_CLK_IDLE_HIGH :set_bit(SPCR,CPOL);break;
		default:break;
	}
	//set read edge
	switch(param->read_edge)
	{
		case SPI_READ_ON_LEADING_EDGE :clear_bit(SPCR,CPHA);break;
		case SPI_READ_ON_TRAILING_EDGE :set_bit(SPCR,CPHA);break;
		default:break;
	}	
	//set data order
	switch(param->data_order)
	{
		case SPI_MSB_FIRST :clear_bit(SPCR,DORD);break;
		case SPI_LSB_FIRST :set_bit(SPCR,DORD);break;
		default:break;
	}	
	//set prescaler
	switch(param->prescaler)
	{
		case SPI_CLK_BY_4 :clear_bit(SPSR,SPI2X);clear_bit(SPCR,SPR1);clear_bit(SPCR,SPR0);break;
		case SPI_CLK_BY_16 :clear_bit(SPSR,SPI2X);clear_bit(SPCR,SPR1);set_bit(SPCR,SPR0);break;
		case SPI_CLK_BY_64 :clear_bit(SPSR,SPI2X);set_bit(SPCR,SPR1);clear_bit(SPCR,SPR0);break;
		case SPI_CLK_BY_128 :clear_bit(SPSR,SPI2X);set_bit(SPCR,SPR1);set_bit(SPCR,SPR0);break;
		case SPI_CLK_BY_2 :set_bit(SPSR,SPI2X);clear_bit(SPCR,SPR1);clear_bit(SPCR,SPR0);break;
		case SPI_CLK_BY_8 :set_bit(SPSR,SPI2X);clear_bit(SPCR,SPR1);set_bit(SPCR,SPR0);break;
		case SPI_CLK_BY_32 :set_bit(SPSR,SPI2X);set_bit(SPCR,SPR1);clear_bit(SPCR,SPR0);break;
		case SPI_CLK_BY_64_2 :set_bit(SPSR,SPI2X);set_bit(SPCR,SPR1);set_bit(SPCR,SPR0);break;
		default:break;
	}	
	//set interrupt
	switch(param->interrupt)
	{
		case SPI_NO_INTERRUPT :clear_bit(SPCR,SPIE);break;
		case SPI_INTERRUPT :
							set_bit(SPCR,SPIE);
							if(param->ptr2func != NULL)
							{
								g_func_ptr=param->ptr2func;
							}
							break;
		default:break;
	}
	//enable spi
	set_bit(SPCR,SPE);
}
//----------------------------------------------------------------
u8 u8Spi_send_receive(u8 data)
{
	SPDR=data;
	while(check_bit(SPSR,SPIF) ==0);
	return SPDR;
}
//---------------------------------------------------------------
ISR(SPI_STC_vect)
{
	if(g_func_ptr !=NULL)
	{
		g_func_ptr();
	}
}
//----------------------------------------------------------
void vSpi_send(u8 data)
{
	SPDR=data;
}
//--------------------------------------------------------------
void vSpi_start_send(void)
{
	clear_bit(PORTB,4);
}
//----------------------------------------------------------------
void vSpi_end_send(void)
{
	set_bit(PORTB,4);
}
//-----------------------------------------------------------