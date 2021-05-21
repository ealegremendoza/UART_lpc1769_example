/*
===============================================================================
 Name        : ADC_lpc1769_example.c
 Author      : ealegremendoza
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/
#include "headers.h"

void SystemInit(void)
{
	InitPLL();
	SysTick_Init(1000);
	UART0_Init(9600);

}
int main(void) {
	SystemInit();
    while(1) {
    	/*	DoSomething();	*/
    	/*	Check systick.c and UART0_lpc1769.c	*/
    }
    return 0 ;
}
