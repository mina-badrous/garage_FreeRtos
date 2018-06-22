/*
 * i2c.c
 *
 * Created: 20/05/2018 10:53:38
 *  Author: BADROUS
 */ 
//------------------------------------------------------------------
#include "i2c.h"
//------------------------------------------------------------------
static I2C_PTR2ISR_t g_isr_ptr;
static u8 g_i2c_init_flag=0;
static void i2c_send(u8 data);
static i2c_error_t i2c_read_ack(i2c_data_t* data);
static i2c_error_t i2c_read_nack(i2c_data_t* data);
//-----------------------------------------------------------------
i2c_init_status_t u8I2c_Init( const i2c_init_param_t* param )
{
	if(g_i2c_init_flag==0)
	{
		u8 index=0;
		u16 freq_temp=0;
		u32 temp=0;
		i2c_init_status_t status=I2C_NO_INIT_ERROR;
		//set the frequency
		//test all prescaler till find the right config
		for(index=0;index<4;index++)
		{
			//calculate TWBR for this configuration
			freq_temp =((float)(param->sysclk)/(param->frequency));
			freq_temp =(freq_temp -(16U));
			temp=(((2U) * pow(4,index)));
			freq_temp= (freq_temp) / (temp) ;
			if(freq_temp>0xFF || freq_temp==0x00)
			{
				status=I2C_FREQ_OUT_OF_RANGE_ERROR;
				continue;
			}
			else
			{
				TWBR=(u8)freq_temp;
				status=I2C_NO_INIT_ERROR;
				break;
			}
		}
		if(status==I2C_FREQ_OUT_OF_RANGE_ERROR)
		{
			return I2C_FREQ_OUT_OF_RANGE_ERROR;
		}
		//set interrupt
	
		//set isr callback
		if(param->ptr !=NULL)
		{
			set_bit(TWCR,TWIE);
			g_isr_ptr=param->ptr;
		}
		//set slave address
		if(param->slave_address !=0)
		{
			TWAR=param->slave_address;
		}
		//enable i2c
		TWCR =(1<<TWEN);
		g_i2c_init_flag=1;
		return I2C_NO_INIT_ERROR;
	}
	else
	{
		
		return I2C_PREV_INIT;
	}	
}
//----------------------------------------------------------------
ISR(TWI_vect)
{
	if(g_isr_ptr != NULL)
	{
		g_isr_ptr();
	}
}
//-------------------------------------------------------------------
i2c_error_t u8I2c_Start(void)
{
	//send start condition and intterrupt flag
	TWCR= (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	//wait till operation finish
	while((TWCR & (1<<TWINT)) ==0);
	//check status 
	if((TWSR & (STATUS_REG_MASK)) !=START_CONDITION_STATE)
	{
		return I2C_START_ERROR;
	}
	else
	{
		return I2C_NO_ERROR;
	}
}
//--------------------------------------------------------------------
i2c_error_t u8I2c_Send_Address(i2c_address_t address,u8 w_r)
{
	switch(w_r)
	{
		case I2C_WRITE  :
						i2c_send(address);
						if((TWSR & (STATUS_REG_MASK)) !=SLAVE_ADDRESS_WRITE_ACK_STATE)
						{
							return I2C_SEND_ADDRESS_ERROR;
						}
						else
						{
							return I2C_NO_ERROR;
						}
						break;
		case I2C_READ   :
						i2c_send(address+I2C_READ);
						if((TWSR & (STATUS_REG_MASK)) !=SLAVE_ADDRESS_READ_ACK_STATE)
						{
							return I2C_SEND_ADDRESS_ERROR;
						}
						else
						{
							return I2C_NO_ERROR;
						}
						break;
		default:return I2C_SEND_ADDRESS_ERROR;
	}
}
//------------------------------------------------------------------
static void i2c_send(u8 data)
{
	//send address and intterrupt flag
	TWDR=data;
	TWCR = (1<<TWINT) | (1<<TWEN);  //start transmition
	//wait till operation finish
	while((TWCR & (1<<TWINT)) ==0);
}
//--------------------------------------------------------------------
i2c_error_t u8I2c_Send_Data(i2c_data_t data)
{
	i2c_send(data);
	if((TWSR & (STATUS_REG_MASK)) !=DATA_SEND_ACk_STATE)
	{
		return I2C_SEND_DATA_ERROR;
	}
	else
	{
		return I2C_NO_ERROR;
	}
	
}
//---------------------------------------------------------------
void vI2c_Stop(void)
{
		TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}
//----------------------------------------------
i2c_error_t u8I2c_Restart(void)
{
		//send start condition and intterrupt flag
		TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA);
		//wait till operation finish
		while((TWCR & (1<<TWINT)) ==0);
		//check status
		if((TWSR & (STATUS_REG_MASK)) !=RESTART_CONDITION_STATE)
		{
			return I2C_START_ERROR;
		}
		else
		{
			return I2C_NO_ERROR;
		}
}
//---------------------------------------------------------
i2c_error_t u8I2c_Read_Data(i2c_data_t* data,u8 count)
{
	i2c_error_t error= I2C_NO_ERROR;
	u8 index=0;
	//check count >0
	if(count==0)
	{
		return I2C_READ_DATA_ERROR;
	}

	for(index=count;index>1;index--)
	{
		error |=i2c_read_ack(data++);
	}
	error |=i2c_read_nack(data);
	if(error!=I2C_NO_ERROR)
	{
		return I2C_READ_DATA_ERROR;
	}else
	{
		return I2C_NO_ERROR;
	}
}
//------------------------------------------------------------
static i2c_error_t i2c_read_ack(i2c_data_t* data)
{
	//start read
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	//wait till end of receive
	while((TWCR & (1<<TWINT)) ==0);
	//check sucsses
	if((TWSR & (STATUS_REG_MASK)) !=DATA_READ_ACK_STATE)
	{
		return I2C_READ_DATA_ERROR;
	}
	else
	{
		*data=TWDR;
		return I2C_NO_ERROR;
	}	
}
//----------------------------------------------------
static i2c_error_t i2c_read_nack(i2c_data_t* data)
{
		//start read
		TWCR = (1<<TWINT) | (1<<TWEN);
		while((TWCR & (1<<TWINT)) ==0);
		//check sucsses
		if((TWSR & (STATUS_REG_MASK)) !=DATA_READ_NACK_STATE)
		{
			return I2C_READ_DATA_ERROR;
		}
		else
		{
			*data=TWDR;
			return I2C_NO_ERROR;
		}
}
//--------------------------------------------------------
i2c_error_t u8I2c_Send_Stream(i2c_data_t* data,u8 count)
{
	u8 index;
	i2c_error_t error=I2C_NO_ERROR;
	if(count==0)
	{
		return I2C_SEND_DATA_ERROR;
	}
	for(index=0;index<count;index++)
	{
		error|=u8I2c_Send_Data(data[index]);
	}
	return error;
	
}
//----------------------------------------------------H L services
i2c_error_t u8I2cWrite_Data_To_Device(i2c_address_t device_address,u16 byte_address,u8 byte_address_length,i2c_data_t data)
{
	i2c_error_t error=I2C_NO_ERROR;
		error=u8I2c_Start();
		if(error !=I2C_NO_ERROR) return error;
		error=u8I2c_Send_Address((u8)device_address,I2C_WRITE);
		if(error !=I2C_NO_ERROR) return error;
		switch(byte_address_length)
		{
		case ADDRESS_LENGTH_1_BYTE :
									error=u8I2c_Send_Data((u8)byte_address);
									if(error !=I2C_NO_ERROR) return error;
									break;
		case ADDRESS_LENGTH_2_BYTE :
									//send high first then low 
									error=u8I2c_Send_Data((u8)((u16)byte_address>>8));
									if(error !=I2C_NO_ERROR) return error;
									error=u8I2c_Send_Data((u8)byte_address);
									if(error !=I2C_NO_ERROR) return error;
									break;		
		default:return I2C_SEND_ADDRESS_ERROR;
		}
		//send data
		error=u8I2c_Send_Data(data);
		if(error !=I2C_NO_ERROR) return error;
		vI2c_Stop();	
		return error;
	
}
//------------------------------------------------------------------------------
i2c_error_t u8I2cRead_Data_From_Device(i2c_address_t device_address,u16 byte_address,u8 byte_address_length,i2c_data_t* data,u8 count)
{
		i2c_error_t error=I2C_NO_ERROR;
		error=u8I2c_Start();
		if(error !=I2C_NO_ERROR) return error;
		switch(byte_address_length)
		{
			case ADDRESS_LENGTH_1_BYTE :
			error=u8I2c_Send_Address((u8)device_address,I2C_WRITE);
			if(error !=I2C_NO_ERROR) return error;
			break;
			case ADDRESS_LENGTH_2_BYTE :
			//send high first then low
			error=u8I2c_Send_Address((u8)((u16)device_address>>8),I2C_WRITE);
			if(error !=I2C_NO_ERROR) return error;
			error=u8I2c_Send_Address((u8)device_address,I2C_WRITE);
			if(error !=I2C_NO_ERROR) return error;
			break;
			default:return I2C_SEND_ADDRESS_ERROR;
		}
		//read data
		error=u8I2c_Read_Data(data,count);
		//stop communication
		vI2c_Stop();
		return error;
}
//------------------------------------------------------------
i2c_error_t u8I2cWrite_Data_To_Device_Stream(i2c_address_t device_address,u16 byte_address,u8 byte_address_length,i2c_data_t* data,u8 count)
{
	i2c_error_t error=I2C_NO_ERROR;
	error=u8I2c_Start();
	if(error !=I2C_NO_ERROR) return error;
	error=u8I2c_Send_Address((u8)device_address,I2C_WRITE);
	if(error !=I2C_NO_ERROR) return error;
	switch(byte_address_length)
	{
		case ADDRESS_LENGTH_1_BYTE :
		error=u8I2c_Send_Data((u8)byte_address);
		if(error !=I2C_NO_ERROR) return error;
		break;
		case ADDRESS_LENGTH_2_BYTE :
		//send high first then low
		error=u8I2c_Send_Data((u8)((u16)byte_address>>8));
		if(error !=I2C_NO_ERROR) return error;
		error=u8I2c_Send_Data((u8)byte_address);
		if(error !=I2C_NO_ERROR) return error;
		break;
		default:return I2C_SEND_ADDRESS_ERROR;
	}
	//send data
	error=u8I2c_Send_Data(*data);
	if(error !=I2C_NO_ERROR) return error;
	vI2c_Stop();
	if(error !=I2C_NO_ERROR) return error;
	u8I2c_Restart();	
	return error;
}