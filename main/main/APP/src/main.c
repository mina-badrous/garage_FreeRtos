/*
 * main.c
 *
 * Created: 30/05/2018 21:12:43
 *  Author: BADROUS
 */ 
//-----------------------------------------------------------------
#define APP_HEADER
#include "main.h"
//-----------------------------------------------------------------

//-------------------------------------------------------------------
static void vCommunication_init(void);

SemaphoreHandle_t uart_mutex;
SemaphoreHandle_t i2c_mutex;
SemaphoreHandle_t xSemaphore;
//TaskHandle_t keypad_task_handler;
//--------------------------------------------------------------
int main(void)
{
	//local var
	xQueueHandle temp;
	xSemaphoreHandle sem_temp;
	
	//init i2c and uart and eeprom and set rtc format to AM/PM
	vCommunication_init();
						
	//CREATE SEMAPHORE
	sem_temp=xSemaphoreCreateBinary();
	vlcd_task_resume_semaphore_set(sem_temp);
	//create mutex
	uart_mutex=xSemaphoreCreateMutex();
	i2c_mutex=xSemaphoreCreateMutex();
	
	//create keypad queue
	temp=xQueueCreate(KEYPAD_TASK_QUEUE_SIZE,KEYPAD_TASK_QUEUE_DATA_SIZE);
	vkeypad_task_q_set(temp);	
	//create led task queue
	temp=xQueueCreate(LED_TASK_QUEUE_SIZE,LED_TASK_QUEUE_DATA_SIZE);
	vLed_task_q_set(temp);
	//create motor task queue
	temp=xQueueCreate(MOTOR_TASK_QUEUE_SIZE,MOTOR_TASK_QUEUE_DATA_SIZE);
	vMotor_task_q_set(temp);
	//create log task queue
	temp=xQueueCreate(LOG_TASK_QUEUE_SIZE,LOG_TASK_QUEUE_DATA_SIZE);
	vLog_task_q_set(temp);
	//create terminal task queue
	temp=xQueueCreate(TERMINAL_TASK_QUEUE_SIZE,TERMINAL_TASK_QUEUE_DATA_SIZE);
	vTerminal_task_q_set(temp);
	
	//create tasks			
		xTaskCreate(vkeypad_task,"keypad_x",100,NULL,4,NULL);
		xTaskCreate(vlcd_task,"lcd_x",100,NULL,3,NULL);
		xTaskCreate(vled_task,"led_x",100,NULL,1,NULL);
		xTaskCreate(vMotor_task,"motor_x",100,NULL,4,NULL);	
		xTaskCreate(vTerminal_task,"term_x",150,NULL,2,NULL);
		xTaskCreate(vLog_task,"log_x",100,NULL,2,NULL);
		
		//start os
		vTaskStartScheduler();
		
    while(1)
    {
		//must be here for debuging
    }
}
//-----------------------------------------------------------
static void vCommunication_init(void)
{
	sei();
	u8 error=0;
		//init communication
		//init spi
		spi_param.ptr2func=spi_interrupt;
		vSpi_init(&spi_param);
		//i2c init
		u8I2c_Init(&i2c_param);
		//init serial
		//set callback
		uart0.rx_ISR=&rx_complete_isr_func;
		//init uart
		vUartInit(&uart0);
		//init eeprom 
		error=u8Init_MCB();
		if(error==EEPROM_LOCKED_MC_STATE)
		{
			//lock state
			vUartTransmitString_program(PSTR("SYSTEM LOCKED PLEASE CONTACT THE DEVELOPER "));
			vUartTransmit(ENTER_KEY);
			while(1)
			{
				if(g_unlock_system_flag==1)
				{
					vEXTERNAL_EEPROM_unlock_state();
					g_unlock_system_flag=0;
					break;
				}
			}
		}

			//set rtc format to 12 
			u8Rtc_Set_format(RTC_12_FORMAT);
}
