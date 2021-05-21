/*
 * systick.c
 *
 *  Created on: 19 mar. 2021
 *      Author: ealegremendoza
 */
#include "systick.h"
/*	@brief:		Inicializa el systick a la frecuencia deseada.
 * 	@param:		frecuencia.	frecuencia en Hz.
 * 	@return:	Nada.
 * 	*/
void SysTick_Init(uint32_t frecuencia)
{
	uint32_t Temporal=0;
	if(frecuencia > 0xffffff)
		return;
	Temporal=CCLK/frecuencia - 1;
	STCURR = 0;
	STRELOAD = Temporal;
	STCTRL = (1<<0)|(1<<1)|(1<<2);
}

/*	@brief:		Rutina que atiende la interrupción del systick
 * 	@param:		Nada.
 * 	@return:	Nada.
 * 	*/
void SysTick_Handler(void)
{
	static uint32_t Divisor_1seg=0;
	static uint32_t Divisor_1min=0;
	char trama[10];
	Divisor_1seg++;
	if(Divisor_1seg==200)
	{
		//1seg!
		Divisor_1seg=0;
		Divisor_1min++;
		Divisor_1min%=60;

		//UART0_TX_Armar_Trama();
		sprintf(trama,"t:%02d\n\r",Divisor_1min);
		UART0_TX_Datos((uint8_t*)trama,0);
	}
}
