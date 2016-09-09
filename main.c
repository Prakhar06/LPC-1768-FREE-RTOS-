
#include "LPC17xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"


#define mainQUEUE_RECEIVE_TASK_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define UARTLog_Task_Priority				( tskIDLE_PRIORITY + 2 )
static xTimerHandle xLEDTimer = NULL;
#define mainLED_TIMER_PERIOD_MS		( 3000UL / portTICK_RATE_MS )
#define mainDONT_BLOCK	(0)
/* Declare a variable that will be incremented by the hook function. */
unsigned long ulIdleCycleCount = 0UL;
/* Idle hook functions MUST be called vApplicationIdleHook(), take no parameters,
and return void. */
void vApplicationIdleHook( void )
{
/* This hook function does nothing but increment a counter. */
ulIdleCycleCount++;
}
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	/* This function will get called if a task overflows its stack. */

	( void ) pxTask;
	( void ) pcTaskName;

	for( ;; );
}

static void prvTask2(void *pvParameters)
{
		  

  while(1)
    {

      LPC_GPIO2->FIOSET = 0xffffffff;     // Make all the Port pins as high 
	vTaskDelay( 2000 / portTICK_RATE_MS ); 
    }

}

static void prvTask1(void *pvParameters)
{
	xTaskCreate( prvTask2, "Task2", configMINIMAL_STACK_SIZE, NULL, mainQUEUE_RECEIVE_TASK_PRIORITY, NULL );

  while(1)
    {

     /* Turn Off all the leds */
		LPC_GPIO2->FIOCLR = 0xffffffff;   
       vTaskDelay( 3000 / portTICK_RATE_MS );

    }
}



/*
// UNCOMMENT THIS FOR TIMER TASK
static void prvLEDTimerCallback( xTimerHandle xTimer )
{
	static int toggle = 0;
	if(toggle == 0) {
		LPC_GPIO2->FIOSET = 0xffffffff;     // Make all the Port pins as high 
		toggle = 1;
	}
	else {
		LPC_GPIO2->FIOCLR = 0xffffffff; 
		toggle = 0;
	}
}

*/



int main(void)
{
	SystemInit();                    //Clock and PLL configuration 
	LPC_PINCON->PINSEL4 = 0x000000; //Configure the Pins for GPIO;
	LPC_GPIO2->FIODIR = 0xffffffff; //Configure the PORT pins as OUTPUT;

	xTaskCreate( prvTask1, "Task1", configMINIMAL_STACK_SIZE, NULL, mainQUEUE_RECEIVE_TASK_PRIORITY, NULL );
	
	
	//xTaskCreate( UARTLogTask, "ULog", configMINIMAL_STACK_SIZE, NULL, UARTLog_Task_Priority, NULL );
	//xLEDTimer = xTimerCreate( 	"LEDTimer", 					/* A text name, purely to help debugging. */
	//							( mainLED_TIMER_PERIOD_MS ),	/* The timer period, in this case 5000ms (5s). */
	//							pdTRUE,						/* This is a one shot timer, so xAutoReload is set to pdFALSE. */
	//							( void * ) 0,					/* The ID is not used, so can be set to anything. */
	//							prvLEDTimerCallback				/* The callback function that switches the LED off. */
	//							);
	
	//xTimerStart( xLEDTimer, mainDONT_BLOCK );
	
	vTaskStartScheduler();

	
	return 0;
}
