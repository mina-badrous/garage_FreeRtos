/*
 * external_eeprom.c
 *
 * Created: 21/05/2018 15:14:54
 *  Author: BADROUS
 */ 
#include "external_eeprom.h"



static void add_user_in_mcb(u8 id);
static void delete_user_from_mcb(u8 id);

static const user_t g_root_user={{"ROOT"},{'0','0','0','0'},ROOT_USER,0x04};
static const u8 g_ex_eeprom_address=EEPROM_DEVICE_ADDRESS;
static const eeprom_memory_control_block_t g_init_mcb={0,0,0,0};
//-----------------------------------------------------------
external_eeprom_error_t u8External_EEPROM_Init(const external_eeprom_param_t* param)
{
	i2c_init_status_t error=I2C_NO_INIT_ERROR;
	error=u8I2c_Init((param->i2c_param));
	if(error !=I2C_NO_INIT_ERROR)
	{
		return EXTRNAL_EEPROM_INIT_ERROR;
	}
	else
	{
		return EXTRNAL_EEPROM_INIT_NO_ERROR;
	}
}
//-------------------------------------------------------
void vExternal_EEPROM_Write(external_eeprom_byte_address_t byte_address,external_eeprom_data* data,u8 count)
{
	u16 index=0;
	u8 temp_address=g_ex_eeprom_address;
	for(index=0;index<count;index++)
	{
		//send device address with p0 as
		u8I2c_Start();
		if(byte_address<=0x00FF)
		{
			//page 0
			u8I2c_Send_Address(temp_address,I2C_WRITE);
			//send byte address
			u8I2c_Send_Data((u8)(byte_address));
		} 
		else
		{
			//page 1
			set_bit(temp_address,1);
			u8I2c_Send_Address(temp_address,I2C_WRITE);
			//send byte address
			u8I2c_Send_Data((u8)(byte_address));
		}
		
		u8I2c_Send_Data(*(data++));
		vI2c_Stop();
		byte_address++;
		_delay_ms(1);
	}		 
}
//--------------------------------------------------------------------------------------------------------
void vExternal_EEPROM_Read(external_eeprom_byte_address_t byte_address,external_eeprom_data* data,u8 count)
{
	u16 index=0;
	u8 temp_address=g_ex_eeprom_address;
	for(index=0;index<count;index++)
	{
		//send device address with p0 as
		u8I2c_Start();
		if(byte_address<=0x00FF)
		{
			//page 0
			u8I2c_Send_Address(temp_address,I2C_WRITE);
			//send byte address
			u8I2c_Send_Data((u8)(byte_address));
			u8I2c_Restart();
			u8I2c_Send_Address(temp_address,I2C_READ);
			u8I2c_Read_Data(data,1);
		}
		else
		{
			//page 1
			set_bit(temp_address,1);
			u8I2c_Send_Address(temp_address,I2C_WRITE);
			//send byte address
			u8I2c_Send_Data((u8)(byte_address));
			u8I2c_Restart();
			u8I2c_Send_Address(temp_address,I2C_READ);
			//read byte 
			u8I2c_Read_Data(data,1);
		}
		
		data++;
		byte_address++;
		vI2c_Stop();
		_delay_ms(1);
	}
}
//--------------------------------------------------------------------------------
void vGet_current_MCB(eeprom_memory_control_block_t* mcb)
{
	u8 checksum=0;
	//get address of mcb
	u8 current_mcb=(u8Get_current_MCB_number())*EEPROM_FIRST_MCB;
	//read from address
	vExternal_EEPROM_Read(current_mcb,(u8 *)&(mcb->current_user_used_flag),4);
	vExternal_EEPROM_Read(current_mcb+4,(u8 *)&(mcb->write_count),2);
	vExternal_EEPROM_Read(current_mcb+6,&(mcb->checksum),1);
	vExternal_EEPROM_Read(current_mcb+7,&(mcb->dummy),1);
	//calculate checksum
	checksum=(u8)(mcb->current_user_used_flag)+(u8)(((u32)mcb->current_user_used_flag)>>8)+
			(u8)(((u32)mcb->current_user_used_flag)>>16)+(u8)(((u32)mcb->current_user_used_flag)>>24);
	//check valid data		
	if(checksum !=mcb->checksum)
	{
		(mcb->dummy)=1;
	}			
}
void vSet_current_MCB(eeprom_memory_control_block_t* mcb)
{
		u8 checksum=0;
		//get the current mcb
		u8 current_mcb=(u8Get_current_MCB_number());
		//increment write count
		(mcb->write_count)++;
		//check over flow and protect from corruption
		if((mcb->write_count) ==0xFFFE)
		{
			current_mcb=current_mcb+1;
			if(current_mcb ==14)
			{
				//error system failed
			}
			vSet_current_MCB_number(current_mcb);
			(mcb->write_count)=0;
		}
		//get the new value of mcb
		current_mcb=(u8Get_current_MCB_number())*EEPROM_FIRST_MCB;
		//calculate checksum
		checksum=(u8)(mcb->current_user_used_flag)+(u8)(((u32)mcb->current_user_used_flag)>>8)
				+(u8)(((u32)mcb->current_user_used_flag)>>16)+(u8)(((u32)mcb->current_user_used_flag)>>24)
				+(u8)(((u16)mcb->write_count)>>8) + (u8)(mcb->write_count);
		//write to eeprom		
		vExternal_EEPROM_Write(current_mcb,(u8 *)&(mcb->current_user_used_flag),4);
		vExternal_EEPROM_Write(current_mcb+4,(u8 *)&(mcb->write_count),2);
		vExternal_EEPROM_Write(current_mcb+6,&checksum,1);
		vExternal_EEPROM_Write(current_mcb+7,&checksum,1);
}
//----------------------------------------------------------------set the pointer to mcb
u8 u8Get_current_MCB_number(void)
{
	u8 data1=0,data2=0;
	vExternal_EEPROM_Read(EEPROM_CURRENT_PTR_MCB_BLOCK_NUMBER_ADDRESS,&data1,1);
	vExternal_EEPROM_Read(EEPROM_CURRENT_PTR_MCB_BLOCK_NUMBER_ADDRESS+1,&data2,1);
	if(data1 ==data2)
	{
		return data1;
	}
	else
	{
		return 0;
	}
}
 void vSet_current_MCB_number(u8 number)
{
	vExternal_EEPROM_Write(EEPROM_CURRENT_PTR_MCB_BLOCK_NUMBER_ADDRESS,&number,1);
	vExternal_EEPROM_Write(EEPROM_CURRENT_PTR_MCB_BLOCK_NUMBER_ADDRESS+1,&number,1);
}
//----------------------------------------------------------------------
u8 u8Init_MCB(void)
{
	u8 first_check=00;
	
	
	//read first byte if not 0xFF
	vExternal_EEPROM_Read(0,&first_check,1);
	if (first_check !=MAGIC_NUMBER)
	{	
		//set flag to magic number
		first_check=MAGIC_NUMBER;
		vExternal_EEPROM_Write(0,&first_check,1);
		//set control block to first block
		vSet_current_MCB_number(FIRST_MCB_NUMBER);
		//set current mcb value
		vSet_current_MCB((eeprom_memory_control_block_t*)&g_init_mcb);
		//add user in block
		add_user_in_mcb(FIRST_USER_ID);
		//add user in memory
		vEXTERNAL_EEPROM_set_user((user_t *)&g_root_user,FIRST_USER_ID);
		//set state of terminal task to factory reset
		vSet_terminal_task_state(TERMINAL_TASK_FACTORY_STATE);
		return EEPROM_FIRST_TIME_INIT;
	}
	else
	{
		//check if locked or not
		vExternal_EEPROM_Read(LOCKED_STATE_ADDRESS,&first_check,1);
		if(first_check ==MAGIC_NUMBER) 
		{
			//locked
			return EEPROM_LOCKED_MC_STATE;
		}
		else
		{
			return EEPROM_ALREADY_INIT;
		}
		
	}
	
}
//--------------------------------------------------------
//set user data with id
void vEXTERNAL_EEPROM_set_user(user_t * user,u8 id)
{
	//calculate checksum
	u8 checksum=0;
	u16 address;
	checksum=user->name[0]+user->name[1]+user->name[2]+user->name[3]+user->name[4]+user->name[5]+user->name[6]+user->name[7]+user->name[8]+user->name[9]
				+user->password[0]+user->password[1]+user->password[2]+user->password[3]+user->type;
	
	address=EEPROM_FIRST_USER+(id *sizeof(user_t));	
	//save in eeprom
		vExternal_EEPROM_Write(address,(u8 *)&(user->name),10);
		vExternal_EEPROM_Write(address+10,(u8 *)&(user->password),4);
		vExternal_EEPROM_Write(address+14,&(user->type),1);
		vExternal_EEPROM_Write(address+15,&checksum,1);

}
//-----------------------------------------------------------
//get user data with id
void vEXTERNAL_EEPROM_get_user(user_t * user,u8 id)
{
	u8 checksum=0;
	u16 address;	
	address=EEPROM_FIRST_USER+(id *sizeof(user_t));
	//save in eeprom
	vExternal_EEPROM_Read(address,(u8 *)&(user->name),10);
	vExternal_EEPROM_Read(address+10,(u8 *)&(user->password),4);
	vExternal_EEPROM_Read(address+14,&(user->type),1);
	vExternal_EEPROM_Read(address+15,&(user->checksum),1);
	checksum=user->name[0]+user->name[1]+user->name[2]+user->name[3]+user->name[4]+user->name[5]+user->name[6]+user->name[7]+user->name[8]+user->name[9]
				+user->password[0]+user->password[1]+user->password[2]+user->password[3]+user->type;
	if(user->checksum !=checksum)
	{
		//error data corrupted
	}
}
//-----------------------------------------------------------------
u8 u8EXTERNAL_EEPROM_search_by_name(unsigned char * name)
{
	u8 index=0;
	u8 result=0;
	user_t user;
	eeprom_memory_control_block_t memory;
	vGet_current_MCB(&memory);
	//get current mcb
	
	for(index=0;index<EEPROM_MAX_USER;index++)
	{
		if(check_bit(memory.current_user_used_flag,index)== 1)
		{
			vEXTERNAL_EEPROM_get_user(&user,index);
			//check name
			result=strcmp((const char *)user.name,(const char *)name);
			if(result ==0)
			{
				return index;
			}
		}
	}
	return USER_NAME_NOT_FOUND;
}
//-------------------------------------------------------------------------
u8 u8EXTERNAL_EEPROM_search_by_password(unsigned char * password,unsigned char* name,u8 * id,u8 * type)
{
	u8 index=0,index2=0;
	u8 result=0;
	user_t user;
	eeprom_memory_control_block_t memory;
	//get current mcb
	vGet_current_MCB(&memory);
	for(index=0;index<EEPROM_MAX_USER;index++)
	{
		if(check_bit(memory.current_user_used_flag,index)== 1)
		{
			vEXTERNAL_EEPROM_get_user(&user,index);
			//check password
			for(index2=0;index2<MAX_USER_PASSWORD;index2++)
			{
				if(user.password[index2] != password[index2])
				{
					result++;
				}
				
			}
			if(result ==0)
			{
				*id= index;
				*type=user.type;
				for(index2=0;index2<MAX_USER_NAME;index2++)
				{
					name[index2]=user.name[index2];
				}
				return PASSWORD_FOUND;
			}
			
		}
		result=0;
	}
	return PASSWORD_NOT_FOUND;
}
//---------------------------------------------------
u8 u8EXTERNAL_EEPROM_add_new_user(user_t * user)
{
	
	u8 index=0;
	u8 available_user=NO_AVAILABLE_USER;
	eeprom_memory_control_block_t memory;
	//get current mcb
	vGet_current_MCB(&memory);
	//check for available user id
	for(index=0;index<EEPROM_MAX_USER;index++)
	{
		if(check_bit(memory.current_user_used_flag,index)== 0)
		{
			available_user=index;
			break;
		}	
	}	
	if(available_user ==NO_AVAILABLE_USER)
	{
		return NO_AVAILABLE_USER;
	}
	else
	{
		//set user in mcb
		 add_user_in_mcb(available_user);
		//set user
		vEXTERNAL_EEPROM_set_user(user,available_user);
		return available_user;
	}			

}


//---------------------------------------------------------
static void add_user_in_mcb(u8 id)
{
	eeprom_memory_control_block_t mcb;
	vGet_current_MCB(&mcb);
	set_bit(mcb.current_user_used_flag,id);
	vSet_current_MCB(&mcb);
}
//----------------------------------
static void delete_user_from_mcb(u8 id)
{
	if(id !=0)
	{
		eeprom_memory_control_block_t mcb;
		vGet_current_MCB(&mcb);
		clear_bit(mcb.current_user_used_flag,id);
		vSet_current_MCB(&mcb);
	}

}
//------------------------------------------------------------------------
void vEXTERNAL_EEPROM_delete_user(u8 id)
{
	if(id!=0 && id<EEPROM_MAX_USER)
	{
		delete_user_from_mcb(id);
	}
	
}
//-----------------------------------
u8 u8EXTERNAL_EEPROM_search_user_byID(u8 id)
{
	eeprom_memory_control_block_t memory;
	if(id>=EEPROM_MAX_USER)
	{
		return USER_NOT_FOUND;
	}
	else
	{
		vGet_current_MCB(&memory);
		if(check_bit(memory.current_user_used_flag,id)==1)
		{
			return USER_FOUND;
		}
		else
		{
			return USER_NOT_FOUND;
		}
	}
}
//-------------------------------------------------------
void vEXTERNAL_EEPROM_edit_user(user_t * user,u8 id)
{
	//set user in mcb to increment mcb
	add_user_in_mcb(id);
	//set user
	vEXTERNAL_EEPROM_set_user(user,id);
}
//-------------------------------------------------------
void vEXTERNAL_EEPROM_reset(void)
{
	u8 first_check=MAGIC_NUMBER;
	vExternal_EEPROM_Write(0,&first_check,1);
	//set control block to first block
	vSet_current_MCB_number(FIRST_MCB_NUMBER);
	//set current mcb value
	vSet_current_MCB((eeprom_memory_control_block_t*)&g_init_mcb);
	//add user in block
	add_user_in_mcb(FIRST_USER_ID);
	//add user in memory
	vEXTERNAL_EEPROM_set_user((user_t *)&g_root_user,FIRST_USER_ID);	
}
//----------------------------------------------------------------------
void vEXTERNAL_EEPROM_lock_state(void)
{
	u8 data=MAGIC_NUMBER;
	vExternal_EEPROM_Write(LOCKED_STATE_ADDRESS,&data,1);
}
//---------------------------------------------------------------------------
void vEXTERNAL_EEPROM_unlock_state(void)
{
	u8 data=0xFF;
	vExternal_EEPROM_Write(LOCKED_STATE_ADDRESS,&data,1);
}