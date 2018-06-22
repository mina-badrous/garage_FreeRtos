/*
 * ecu_keypad.c
 *
 * Created: 24/05/2018 15:43:13
 *  Author: BADROUS
 */ 
#include "ecu_keypad.h"
//-------------------------------------------------------------------
/*
  const static unsigned char s_keypad_mapping[4][4]= {{'7','8','9','/'},
													  {'4','5','6','*'},
											          {'1','2','3','-'},
									                  {'C','0','=','+'}};
														  */
  const static unsigned char s_keypad_mapping[4][4]= {{'1','2','3','/'},
													  {'4','5','6','*'},
													  {'7','8','9','-'},
													  {'*','0','#','+'}};
//-----------------------------------------------------------------------
void vEcu_KeypadInit(ecu_keypad_init_param_t* param)
{
	//make output =1 no output state
	param->row0->output_init_value=STD_LOGIC_HIGH;
	param->row1->output_init_value=STD_LOGIC_HIGH;
	param->row2->output_init_value=STD_LOGIC_HIGH;
	param->row2->output_init_value=STD_LOGIC_HIGH;
	//make input pullup
	vGpio_Init((param->row0));
	vGpio_Init((param->row1));
	vGpio_Init((param->row2));
	vGpio_Init((param->row3));
	vGpio_Init((param->col0));
	vGpio_Init((param->col1));
	vGpio_Init((param->col2));
	vGpio_Init((param->col3));
	
}
//----------------------------------------------------------------------
unsigned char ucEcu_KeypadScan(ecu_keypad_init_param_t* param)
{
	//pseudo code
	uint8_t row=0;
	uint8_t line = NO_VALID_INPUT;
	unsigned char result = NO_VALID_INPUT;
	gpio_write_param_t temp;
	gpio_read_param_t temp2;
	u8 col_0,col_1,col_2,col_3;
	//loop on rows
	for (row=0;row<4;row++)
	{
		//set rows output to no output state(1)
		//make output =1 no output state
		temp.port=param->row0->port;
		temp.pin=param->row0->pin;
		temp.output=STD_LOGIC_HIGH;
		vGpio_Write(&temp);
		
		temp.port=param->row1->port;
		temp.pin=param->row1->pin;
		temp.output=STD_LOGIC_HIGH;
		vGpio_Write(&temp);
		
		temp.port=param->row2->port;
		temp.pin=param->row2->pin;
		temp.output=STD_LOGIC_HIGH;
		vGpio_Write(&temp);
				
		temp.port=param->row3->port;
		temp.pin=param->row3->pin;
		temp.output=STD_LOGIC_HIGH;
		vGpio_Write(&temp);
		
		//set certain row to output state (0)
		switch(row)
		{
			case 0:
							temp.port=param->row0->port;
							temp.pin=param->row0->pin;
							temp.output=STD_LOGIC_LOW;
							vGpio_Write(&temp);
							break;
			case 1:
							temp.port=param->row1->port;
							temp.pin=param->row1->pin;
							temp.output=STD_LOGIC_LOW;
							vGpio_Write(&temp);
							break;
			case 2:
							temp.port=param->row2->port;
							temp.pin=param->row2->pin;
							temp.output=STD_LOGIC_LOW;
							vGpio_Write(&temp);
							break;
			case 3:
							temp.port=param->row3->port;
							temp.pin=param->row3->pin;
							temp.output=STD_LOGIC_LOW;
							vGpio_Write(&temp);
							break;
			default:break;
		}
		//check on lines
		temp2.port=param->col0->port;
		temp2.pin=param->col0->pin;
		col_0=Gpio_Read(&temp2);
		
		temp2.port=param->col1->port;
		temp2.pin=param->col1->pin;
		col_1=Gpio_Read(&temp2);
				
		temp2.port=param->col2->port;
		temp2.pin=param->col2->pin;
		col_2=Gpio_Read(&temp2);
						
		temp2.port=param->col3->port;
		temp2.pin=param->col3->pin;
		col_3=Gpio_Read(&temp2);
		
		if (col_0==IS_VALID){line=0;}
		else if (col_1==IS_VALID){line=1;}
		else if (col_2==IS_VALID){line=2;}
		else if (col_3==IS_VALID){line=3;}
		else {/* MISRA */}
		//check for input validity
		if (line != NO_VALID_INPUT)
		{	
			//save the mapped key[row][line]
			result = s_keypad_mapping[row][line];
			//break from the loop
			break;
		}
		
	}
	//return the mapped key
	return result;
}
//-----------------------------------------------------------------------
keypad_switch_state_t ucEcu_KeypadScanState(ecu_keypad_init_param_t* param,unsigned char* button)
{
		 static keypad_switch_state_t last_state=KP_SWITCH_RELEASED_STATE;
		 static unsigned char buttonval=NO_VALID_INPUT;
		 unsigned char value_read=NO_VALID_INPUT;
		 
		 switch(last_state)
		 {
			 case KP_SWITCH_RELEASED_STATE :
			 value_read=ucEcu_KeypadScan(param);
			 if(value_read ==(NO_VALID_INPUT))
			 {
				 last_state=KP_SWITCH_RELEASED_STATE;
			 }
			 else
			 {
				 last_state=KP_SWITCH_PRE_PRESSED_STATE;
			 }
			 break;
			 case KP_SWITCH_PRE_PRESSED_STATE:
			 value_read=ucEcu_KeypadScan(param);
			 if(value_read ==(NO_VALID_INPUT))
			 {
				 last_state=KP_SWITCH_PRE_RELEASED_STATE;
			 }
			 else
			 {
				 last_state=KP_SWITCH_PRESSED_STATE;
			 }
			 break;
			 case KP_SWITCH_PRESSED_STATE:
			 value_read=ucEcu_KeypadScan(param);
			 if(value_read ==(NO_VALID_INPUT))
			 {
				 last_state=KP_SWITCH_PRE_RELEASED_STATE;
			 }
			 else
			 {
				 last_state=KP_SWITCH_PRESSED_STATE;
			 }
			 break;
			 case KP_SWITCH_PRE_RELEASED_STATE:
			 value_read=ucEcu_KeypadScan(param);
			 if(value_read ==(NO_VALID_INPUT))
			 {
				 last_state=KP_SWITCH_RELEASED_STATE;
			 }
			 else
			 {
				 last_state=KP_SWITCH_PRE_PRESSED_STATE;
			 }
			 break;
			 default:break;
		 }
		 //for pre pressed value be the same pre released value
		 if(last_state==KP_SWITCH_PRE_PRESSED_STATE)
		 {
			 buttonval=value_read;
			 *button=buttonval;
		 }
		 if(last_state==KP_SWITCH_PRE_RELEASED_STATE)
		 {
			 
			 *button=buttonval;
		 }
		 
		 return last_state;
}
//--------------------------------------------------------------------

