/*
 * keypad_task.c
 *
 * Created: 31/05/2018 4:54:05
 *  Author: BADROUS
 */
//------------------------------------------------------------- 
#include "keypad_task.h"
//---------------------------------------------------------------
static xQueueHandle g_keypad_q;
//--------------------------------------------------------------
void vkeypad_task(void *pvParameters)
{
	//init
	portBASE_TYPE que_state;
	u8 button=NO_VALID_INPUT;
	keypad_switch_state_t kp_state=KP_SWITCH_RELEASED_STATE;
	vEcu_KeypadInit(&keypad_param);
	//do task
	while(1)
	{
		//scan keypad
		kp_state=ucEcu_KeypadScanState(&keypad_param,&button);
		//check valid input
		if(kp_state ==KP_SWITCH_PRE_PRESSED_STATE )
		{
			//send to queue
			 que_state=xQueueSendToBack(g_keypad_q,(void*)&button,20);
			 if(que_state !=pdPASS)
			 {
				 //error
			 }

		}
		else
		{
			vTaskDelay( 50 );
		}		
	}
}
//----------------------------------------------------------------
void vkeypad_task_q_set(xQueueHandle key)
{
	g_keypad_q=key;
}
//----------------------------------------------------------------
xQueueHandle xkeypad_task_q_get(void)
{
	return g_keypad_q;
}
//--------------------------------------------------------------