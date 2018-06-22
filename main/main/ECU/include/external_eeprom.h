/*
 * external_eeprom.h
 *
 * Created: 21/05/2018 15:14:38
 *  Author: BADROUS
 */ 


#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_
//---------------------------------------------------------------------
#include "i2c_Cfg.h"
#include <string.h>
#include "main.h"
#include "terminal_task.h"
//-----------------------------------------------------------------------
#define  EEPROM_FIRST_TIME_INIT 1U
#define  EEPROM_ALREADY_INIT 0U
#define EEPROM_LOCKED_MC_STATE 2U


#define FIRST_MCB_NUMBER 1
#define FIRST_USER_ID 0
#define EEPROM_DEVICE_ADDRESS 0xA0
typedef u8 external_eeprom_address_t;
typedef u16 external_eeprom_byte_address_t;
typedef i2c_data_t external_eeprom_data;
//---------------------------------------------------------------
#define USER_NAME_NOT_FOUND 0xff

#define USER_VALID 0
#define USER_INVALID 0xFF

#define EXTERNAL_EEPROM_MAX_STREAM_BYTE 16

#define MAX_USER_NAME 9U
#define MAX_USER_PASSWORD 4U

#define USER_FOUND 0
#define USER_NOT_FOUND 1

#define PASSWORD_FOUND 0U
#define PASSWORD_NOT_FOUND 0xFF

#define MAGIC_NUMBER 0xAA
#define LOCKED_STATE_ADDRESS 4U


#define ROOT_USER 0U
#define	ADMIN_USER 1U
#define	USER 2U
#define INVALID_USER 3U
//-------------------------------------------------------------memory address
#define USER_ADD_SUCCESS 0U
#define NO_AVAILABLE_USER 0xff
#define EEPROM_FIRST_TIME_FLAG_ADDRESS 0U
#define EEPROM_CURRENT_PTR_MCB_BLOCK_NUMBER_ADDRESS 1U
#define EEPROM_FIRST_MCB 8U
#define EEPROM_MAX_MCB 13U
#define EEPROM_FIRST_USER 112U
#define EEPROM_MAX_USER 25U
//------------------------------------------------------------
typedef enum
{
	EXTRNAL_EEPROM_INIT_NO_ERROR=0,
	EXTRNAL_EEPROM_INIT_ERROR
} external_eeprom_error_t;

typedef struct
{
	const i2c_init_param_t*					   i2c_param;
	external_eeprom_address_t			   eeprom_address;
} external_eeprom_param_t;
//----------------------------------------------------------------------
typedef struct
{
	unsigned char name[MAX_USER_NAME+1];
	unsigned char password[MAX_USER_PASSWORD];
	u8            type;
	u8            checksum;
} user_t;
//----------------------------------------------------------
typedef struct
{
	u32     current_user_used_flag;
	u16     write_count;
	u8      checksum;
	u8      dummy;
} eeprom_memory_control_block_t;


//----------------------------------------------------------------
external_eeprom_error_t u8External_EEPROM_Init(const external_eeprom_param_t* param);
void vExternal_EEPROM_Write(external_eeprom_byte_address_t byte_address,external_eeprom_data* data,u8 count);
void vExternal_EEPROM_Read(external_eeprom_byte_address_t byte_address,external_eeprom_data* data,u8 count);
//----------------------------------------------------------------HIGH LEVEL FUNCTIONS
u8 u8Init_MCB(void);

u8 u8Get_current_MCB_number(void);
void vSet_current_MCB_number(u8 number);

void vGet_current_MCB(eeprom_memory_control_block_t* mcb); /*(mcb from 1 >> 13)  if mcb->dummy != mcb->checksum error*/
void vSet_current_MCB(eeprom_memory_control_block_t* mcb);

//set user data with id 
void vEXTERNAL_EEPROM_set_user(user_t * user,u8 id);
//get user data with id
void vEXTERNAL_EEPROM_get_user(user_t * user,u8 id);/*id from 0 >> 24*/
//search with password and return name
u8 u8EXTERNAL_EEPROM_search_by_name(unsigned char * name);
u8 u8EXTERNAL_EEPROM_search_by_password(unsigned char * password,unsigned char* name,u8 * id,u8 * type);
//add new user 
u8 u8EXTERNAL_EEPROM_add_new_user(user_t * user);
//delete user
void vEXTERNAL_EEPROM_delete_user(u8 id);
//find user 
u8 u8EXTERNAL_EEPROM_search_user_byID(u8 id);
void vEXTERNAL_EEPROM_edit_user(user_t * user,u8 id);
void vEXTERNAL_EEPROM_reset(void);

void vEXTERNAL_EEPROM_lock_state(void);
void vEXTERNAL_EEPROM_unlock_state(void);

#endif /* EXTERNAL_EEPROM_H_ */