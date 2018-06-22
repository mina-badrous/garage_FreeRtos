/*
 * i2c.h
 *
 * Created: 20/05/2018 10:53:24
 *  Author: BADROUS
 */ 

//--------------------------------------------------------------
#ifndef I2C_H_
#define I2C_H_
//----------------------------------------------------------------
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "int.h"
#include "bitwise.h"
#include <math.h>
//--------------------------------------------------------------------
#define ADDRESS_LENGTH_1_BYTE 1U
#define ADDRESS_LENGTH_2_BYTE 2U
#define STATUS_REG_MASK 0xF8
#define I2C_WRITE 0U
#define I2C_READ 1U
typedef u8 i2c_init_status_t;
typedef u8 i2c_data_t;
typedef u32 i2c_frequency_t;
typedef void (*I2C_PTR2ISR_t)(void);
typedef u8 i2c_address_t;
typedef u32 i2c_cpu_frequency_t;

typedef enum
{
	//----------------------------------------master send
	START_CONDITION_STATE=0x08,
	RESTART_CONDITION_STATE=0x10,
	SLAVE_ADDRESS_WRITE_ACK_STATE=0x18,
	SLAVE_ADDRESS_WRITE_NACK_STATE=0x20,
	DATA_SEND_ACk_STATE=0x28,
	DATA_SEND_NACk_STATE=0x30,
	ARBITRATION_LOST_STATE=0x38,
	//------------------------------------------master receive
	SLAVE_ADDRESS_READ_ACK_STATE=0x40,
	SLAVE_ADDRESS_READ_NACK_STATE=0x48,
	DATA_READ_ACK_STATE=0x50,
	DATA_READ_NACK_STATE=0x58
	//------------------------------------------slave receive
	//------------------------------------------slave send
	
} i2c_status_t;
typedef enum
{
	I2C_DISABLE_INTERRUPT=0,
	I2C_ENABLE_INTERRUPT
} i2c_interrupt_t;

typedef enum
{
	I2C_PRESCALER_BY_1=0,
	I2C_PRESCALER_BY_4,
	I2C_PRESCALER_BY_16,
	I2C_PRESCALER_BY_64
} i2c_prescaler_t;

typedef enum
{
	I2C_NO_INIT_ERROR=0,
	I2C_FREQ_OUT_OF_RANGE_ERROR,
	I2C_NO_ERROR,
	I2C_START_ERROR,
	I2C_SEND_ADDRESS_ERROR,
	I2C_SEND_DATA_ERROR,
	I2C_READ_DATA_ERROR,
	I2C_PREV_INIT	
} i2c_error_t;

typedef struct
{
	i2c_cpu_frequency_t	 sysclk;
	i2c_frequency_t		 frequency;//frequency = (sysclk/(16+(2*(TWBR)*(4^TWPS))))
	i2c_prescaler_t      prescaler;
	i2c_interrupt_t		 interrupt;
	i2c_address_t		 slave_address;
	I2C_PTR2ISR_t		 ptr;
} i2c_init_param_t;


//***************************FUNCTIONS PROTOTYPE**********************************
i2c_init_status_t u8I2c_Init(const i2c_init_param_t* param);
i2c_error_t u8I2c_Start(void);
i2c_error_t u8I2c_Restart(void);
void vI2c_Stop(void);
i2c_error_t u8I2c_Send_Address(i2c_address_t address,u8 w_r);
i2c_error_t u8I2c_Send_Data(i2c_data_t data);
i2c_error_t u8I2c_Send_Stream(i2c_data_t* data,u8 count);
i2c_error_t u8I2c_Read_Data(i2c_data_t* data,u8 count);
//------------------------------------------------------service
i2c_error_t u8I2cWrite_Data_To_Device(i2c_address_t device_address,u16 byte_address,u8 byte_address_length,i2c_data_t data);
i2c_error_t u8I2cWrite_Data_To_Device_Stream(i2c_address_t device_address,u16 byte_address,u8 byte_address_length,i2c_data_t* data,u8 count);
i2c_error_t u8I2cRead_Data_From_Device(i2c_address_t device_address,u16 byte_address,u8 byte_address_length,i2c_data_t* data,u8 count);
//--------------------------------------------------------------------
#endif /* I2C_H_ */