/*
 * spi.h
 *
 * Created: 17/06/2018 2:28:09
 *  Author: BADROUS
 */ 
#ifndef SPI_H_
#define SPI_H_
//--------------------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
#include "int.h"
#include "bitwise.h"
#include <stdlib.h>
//----------------------------------------------------------------
typedef enum
{
	SPI_SLAVE=0,
	SPI_MASTER
} spi_mode_t;
//-----------------------------------------------------------------------
typedef enum
{
	SPI_NO_INTERRUPT=0,
	SPI_INTERRUPT
} spi_interrupt_mode_t;
//------------------------------------------------------------------------
typedef void (*ptr2Spi)(void);
typedef enum
{
	SPI_MSB_FIRST=0,
	SPI_LSB_FIRST
} spi_data_order_t;

typedef enum
{
	SPI_CLK_IDLE_LOW=0,
	SPI_CLK_IDLE_HIGH
} spi_idle_state_t;

typedef enum
{
	SPI_READ_ON_LEADING_EDGE=0,
	SPI_READ_ON_TRAILING_EDGE
} spi_read_edge_t;

typedef enum
{
	SPI_CLK_BY_4=0,
	SPI_CLK_BY_16,
	SPI_CLK_BY_64,
	SPI_CLK_BY_128,
	SPI_CLK_BY_2,
	SPI_CLK_BY_8,
	SPI_CLK_BY_32,
	SPI_CLK_BY_64_2	
} spi_prescaler_t;
//-------------------------------------------------------------------

typedef struct
{
	spi_mode_t					mode;
	spi_idle_state_t			idle;
	spi_read_edge_t				read_edge;
	spi_data_order_t			data_order;
	spi_prescaler_t				prescaler;
	spi_interrupt_mode_t		interrupt;
	ptr2Spi						ptr2func;
} spi_init_param_t;
//-----------------------------------------------------------------------
void vSpi_init(spi_init_param_t * param);


void vSpi_start_send(void);
void vSpi_end_send(void);

u8 u8Spi_send_receive(u8 data);

void vSpi_send(u8 data);

#endif /* SPI_H_ */