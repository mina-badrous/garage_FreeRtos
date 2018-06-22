/*
 * ecu_lcd.c
 *
 * Created: 26/05/2018 18:22:19
 *  Author: BADROUS
 */ 
#include "ecu_lcd.h"
//----------------------------------------------------------
static void SvLcdSend(u8 type,u8 data);
static void SvLatch_data(void);
//------------------------------------
static ecu_lcd_init_param_t* g_param;
//---------------------------------------------------------------
void vEcu_LcdInit(ecu_lcd_init_param_t* param)
{
	//save pointer to struct
	g_param=param;
	//init gpio 
	vGpio_Init(g_param->d4);
	vGpio_Init(g_param->d5);
	vGpio_Init(g_param->d6);
	vGpio_Init(g_param->d7);
	
	vGpio_Init(g_param->rs);
	vGpio_Init(g_param->e);
	//delay to reach vcc level
	_delay_ms(20);
	//send init cmds
	SvLcdSend(CMD,LCD_CMD_MODE4_INIT0);
	SvLcdSend(CMD,LCD_CMD_MODE4_INIT1);
	SvLcdSend(CMD,LCD_CMD_MODE4_2LINES);
	SvLcdSend(CMD,LCD_CMD_DISPLAY_ON_CURSOR_OFF);
}
//--------------------------------------------------------------------
void vEcu_LcdSendChar(char data)
{
	SvLcdSend(DATA,data);
}
//------------------------------------------------------------------
u8 u8Ecu_LcdSendString(char* str_ptr)
{
	//check if no char send
	if(str_ptr ==NULL)
	{
		return LCD_NO_CHAR_TO_PRINT ;
	}
	else
	{   //print till null char
		while(*str_ptr !='\0')
		{
			SvLcdSend(DATA,*str_ptr);
			str_ptr++;
		}
		return LCD_NO_ERROR ;
	}
}
//------------------------------------------------------------------
void vEcu_LcdSendCmd(char data)
{
	SvLcdSend(CMD,data);
}
//------------------------------------------------------------------
void vEcu_LcdClearDisplay(void)
{
	SvLcdSend(CMD,LCD_CMD_CLEAR_DISPLAY);
	_delay_ms(100);
}
//-----------------------------------------------------------------
u8 u8Ecu_LcdGoToXy(u8 line,u8 column)
{
	//limit check
	switch(line)
	{
		case LCD_FIRST_LINE :
		if(column >=0 && column <=15)
		{
			SvLcdSend(CMD,FIRST_LINE_ADDRESS + column);
		}
		else
		{
			return LCD_ERROR_COLUMN_OUT_OF_RANGE ;
		}
		break;
		case LCD_SECOND_LINE :
		if(column >=0 && column <=15)
		{
			SvLcdSend(CMD,SECOND_LINE_ADDRESS + column);
		}
		else
		{
			return LCD_ERROR_COLUMN_OUT_OF_RANGE ;
		}
		break;
		default:return LCD_ERROR_ROW_OUT_OF_RANGE ;
	}
	return LCD_NO_ERROR ;
}
//-----------------------------------------------high level interface
u8 u8Ecu_LcdPrint(u8 data,u8 line,u8 position)
{
	u8 l=0;
	if(line>1 || line<0)
	{
		return LCD_ERROR_ROW_OUT_OF_RANGE;   //error
	}
	if(position>15 || position<0)
	{
		position=position % 16;  //error
	}
	switch (line)
	{
		case 0:
		l=0x80 +position;
		break;
		default:
		l=0xc0+position;
		break;
		
	}
	SvLcdSend(CMD,l);    //position    11
	SvLcdSend(DATA,data);  //data
	return LCD_NO_ERROR;
}
//----------------------------------------------------------------------
//--------------------------------------------------------------
static void SvLcdSend(u8 type,u8 data)
{
		gpio_write_param_t temp;
	//make RS =0 as cmd and = 1 as data
	if(type==CMD)
	{
		//clear rs
		temp.port=g_param->rs->port;
		temp.pin=g_param->rs->pin;
		temp.output=STD_LOGIC_LOW;
		vGpio_Write(&temp);
	}
	else
	{
		//set rs
		temp.port=g_param->rs->port;
		temp.pin=g_param->rs->pin;
		temp.output=STD_LOGIC_HIGH;
		vGpio_Write(&temp);
	}
	
	//high nibble data send
	
	if(check_bit(data,4))
	{
		temp.port=g_param->d4->port;
		temp.pin=g_param->d4->pin;
		temp.output=STD_LOGIC_HIGH;
		vGpio_Write(&temp);
	}
	else
	{
		temp.port=g_param->d4->port;
		temp.pin=g_param->d4->pin;
		temp.output=STD_LOGIC_LOW;
		vGpio_Write(&temp);
	}
	if(check_bit(data,5))
	{
		temp.port=g_param->d5->port;
		temp.pin=g_param->d5->pin;
		temp.output=STD_LOGIC_HIGH;
		vGpio_Write(&temp);
	}
	else
	{
		temp.port=g_param->d5->port;
		temp.pin=g_param->d5->pin;
		temp.output=STD_LOGIC_LOW;
		vGpio_Write(&temp);
	}
	if(check_bit(data,6))
	{
		temp.port=g_param->d6->port;
		temp.pin=g_param->d6->pin;
		temp.output=STD_LOGIC_HIGH;
		vGpio_Write(&temp);
	}
	else
	{
		temp.port=g_param->d6->port;
		temp.pin=g_param->d6->pin;
		temp.output=STD_LOGIC_LOW;
		vGpio_Write(&temp);
	}
	if(check_bit(data,7))
	{
		temp.port=g_param->d7->port;
		temp.pin=g_param->d7->pin;
		temp.output=STD_LOGIC_HIGH;
		vGpio_Write(&temp);
	}
	else
	{
		temp.port=g_param->d7->port;
		temp.pin=g_param->d7->pin;
		temp.output=STD_LOGIC_LOW;
		vGpio_Write(&temp);
	}
	//latch data
	SvLatch_data();
	//delay
	_delay_ms(2);
	//lower nibble data
	if(check_bit(data,0))
	{
		temp.port=g_param->d4->port;
		temp.pin=g_param->d4->pin;
		temp.output=STD_LOGIC_HIGH;
		vGpio_Write(&temp);
	}
	else
	{
		temp.port=g_param->d4->port;
		temp.pin=g_param->d4->pin;
		temp.output=STD_LOGIC_LOW;
		vGpio_Write(&temp);
	}
	if(check_bit(data,1))
	{
		temp.port=g_param->d5->port;
		temp.pin=g_param->d5->pin;
		temp.output=STD_LOGIC_HIGH;
		vGpio_Write(&temp);
	}
	else
	{
		temp.port=g_param->d5->port;
		temp.pin=g_param->d5->pin;
		temp.output=STD_LOGIC_LOW;
		vGpio_Write(&temp);
	}
	if(check_bit(data,2))
	{
		temp.port=g_param->d6->port;
		temp.pin=g_param->d6->pin;
		temp.output=STD_LOGIC_HIGH;
		vGpio_Write(&temp);
	}
	else
	{
		temp.port=g_param->d6->port;
		temp.pin=g_param->d6->pin;
		temp.output=STD_LOGIC_LOW;
		vGpio_Write(&temp);
	}
	if(check_bit(data,3))
	{
		temp.port=g_param->d7->port;
		temp.pin=g_param->d7->pin;
		temp.output=STD_LOGIC_HIGH;
		vGpio_Write(&temp);
	}
	else
	{
		temp.port=g_param->d7->port;
		temp.pin=g_param->d7->pin;
		temp.output=STD_LOGIC_LOW;
		vGpio_Write(&temp);
	}
	//latch data
	SvLatch_data();
}
//----------------------------------------------------------
static void SvLatch_data( void )
{
	//make E HIGH
	gpio_write_param_t temp;
	temp.port=g_param->e->port;
	temp.pin=g_param->e->pin;
	temp.output=STD_LOGIC_HIGH;
	vGpio_Write(&temp);
	
	//delay
	_delay_us(20);
	
	//make E LOw
	temp.port=g_param->e->port;
	temp.pin=g_param->e->pin;
	temp.output=STD_LOGIC_LOW;
	vGpio_Write(&temp);
	_delay_us(20);
}
//--------------------------------------------------------------------
void vEcu_LcdSendPassword(char data)
{
	vEcu_LcdSendChar(data);
	_delay_ms(300);
	vEcu_LcdSendCmd(LCD_CMD_CURSOR_MOVE_LEFT);
	_delay_ms(20);
	vEcu_LcdSendChar(' ');
	_delay_ms(300);
	vEcu_LcdSendCmd(LCD_CMD_CURSOR_MOVE_LEFT);
	_delay_ms(20);
	vEcu_LcdSendChar('*');
}
//----------------------------------------------------------------
void vEcu_LcdRemovePassword(void)
{
	vEcu_LcdSendCmd(LCD_CMD_CURSOR_MOVE_LEFT);
	vEcu_LcdSendChar(' ');
	vEcu_LcdSendCmd(LCD_CMD_CURSOR_MOVE_LEFT);
}