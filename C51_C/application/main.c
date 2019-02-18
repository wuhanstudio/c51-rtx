#include <reg52.h>
#include <rtx51tny.h>
#include "lcd.h"                

#include "uart.h"
#define OSC_FREQ            11059200
#define BAUD_RATE           9600
#define NEW_LINE_THRESHOLD  1


sbit LED1 = P1^0;
sbit LED2 = P1^2;
sfr WDTRST= 0x0A6;

void main_job() _task_ 0  
{
	//PS = 1;
	// WDTRST = 0x01E;
	// WDTRST = 0x0E1;
	os_create_task(1);		//task1 	
  os_create_task(2);		//task2 	
	os_create_task(3);		//task3
	os_create_task(4);		//task4 	
	os_create_task(5);		//task5 	
	os_delete_task (0);    /* Delete the Startup Task */
}

void led1_job() _task_ 1 {
	int i;
	LED1 = 0;	
	for (;;)   		
	{ 
		LED1 = !LED1;
		for(i=0; i<10;i++)
			os_wait(K_TMO,100, 0);
	}
} 

void led2_job() _task_ 2 
{
	int i;
	LED2 = 0;	
	for (;;)   		
	{ 
		LED2 = !LED2; 		
		for(i=0; i<15;i++)
			os_wait(K_TMO, 100, 0);
	}
} 

void usart_job(void) _task_ 3
{
	Serialbegin(OSC_FREQ, BAUD_RATE);       // Set Baud Rate
	Serialflush();                // Clear the buffers
	Serialprint("uart test\n\r"); // Print a string
	setSerialinterrupt();         // Enable Serial Interrupt
	while(1)
	{
		while(uartNewLineCount<NEW_LINE_THRESHOLD); // Wait until new line count reaches threshold
		Serialprint(uartReadBuffer);                // Print the uart read buffer
		Serialflush();                              // Flush the buffer
	}
}

#define String1  "  Hello  World  "
#define String2  "    Hai 8051    "

void lcd_job(void) _task_ 4
{
	int i;
	lcdBegin();                       // Initiate the LCD
	lcdClear();
	os_wait(K_TMO, 50, 0);	
	while(1)
	{
		lcdSetCursor(1,0);
		lcdPrint(String1);
		for(i=0; i<10;i++)
			os_wait(K_TMO, 200, 0);
    lcdSetCursor(2,0);
    lcdPrint(String2);
    for(i=0; i<10;i++)
			os_wait(K_TMO, 200, 0);
    lcdClear();
    for(i=0; i<10;i++)
			os_wait(K_TMO, 200, 0);
	}
}

void watch_dog_job() _task_ 5
{
	while(1)
	{
		WDTRST = 0x01E;
		WDTRST = 0x0E1;
		os_wait(K_TMO, 10, 0);
	}
}