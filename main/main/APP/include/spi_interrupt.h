/*
 * spi_interrupt.h
 *
 * Created: 19/06/2018 17:58:02
 *  Author: BADROUS
 */ 


#ifndef SPI_INTERRUPT_H_
#define SPI_INTERRUPT_H_

#include "spi_Cfg.h"

void spi_interrupt(void);

extern volatile u8 g_unlock_system_flag;

#endif /* SPI_INTERRUPT_H_ */