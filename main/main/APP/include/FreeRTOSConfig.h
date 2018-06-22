/*
 * FreeRTOSConfig.h
 *
 * Created: 14/05/2018 20:38:53
 *  Author: BADROUS
 */ 
//---------------------------------------------------------------------------
#ifndef FREERTOSCONFIG_H_
#define FREERTOSCONFIG_H_
//-------------------------------------------------------------------
#define configCPU_CLOCK_HZ              	16000000UL
#define configUSE_TICKLESS_IDLE         	0
#define configTICK_RATE_HZ              	( ( TickType_t ) 1000 )
#define configUSE_PREEMPTION            	1
#define configUSE_IDLE_HOOK             	0
#define configUSE_TICK_HOOK             	0
#define configMAX_PRIORITIES            	( 5 )			 /* system priorities are from 0-4 and 4 is the highest*/
#define configMINIMAL_STACK_SIZE        	( ( unsigned short ) 100 )
#define configTOTAL_HEAP_SIZE           	( ( size_t ) ( 1500 ) )  /*1024 edited by me*/
#define configMAX_TASK_NAME_LEN         	( 10 )
#define configUSE_16_BIT_TICKS          	1
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY    10
//----------------------------------------------------------------------
#define configUSE_MUTEXES 1 
#define configUSE_TIME_SLICING 	1
#define INCLUDE_vTaskDelay      	1
//-------------------------------------------------------------------------------
#endif /* FREERTOSCONFIG_H_ */