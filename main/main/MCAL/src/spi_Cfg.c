/*
 * spi_Cfg.c
 *
 * Created: 17/06/2018 2:28:34
 *  Author: BADROUS
 */ 
#include "spi_Cfg.h"

spi_init_param_t spi_param =
{
	SPI_SLAVE,						//mode;
	SPI_CLK_IDLE_LOW,			//idle;
	SPI_READ_ON_LEADING_EDGE,				//read_edge;
	SPI_MSB_FIRST,			//data_order;
	SPI_CLK_BY_128,			//prescaler;
	SPI_INTERRUPT,		//interrupt;
	NULL						//ptr2func;
};