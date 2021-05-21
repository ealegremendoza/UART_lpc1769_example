/*
 * GPIO.c
 *
 *  Created on: 19 mar. 2021
 *      Author: ealegremendoza
 */

#include "GPIO.h"
#if CANT_ENTRADAS_DIG>0
STRUCT_ENTRADAS_DIGITALES  Entrada[CANT_ENTRADAS_DIG];
#endif
#if CANT_SALIDAS_DIG > 0
STRUCT_SALIDAS_DIGITALES  Salida[CANT_SALIDAS_DIG];
#endif

void GPIO_Func(uint32_t Port, uint32_t Pin, uint32_t Function)
{
	uint32_t Temporal = Port*2;
	if(Pin>15)
	{
		Temporal++;
		Pin-=16;
	}
	PINSEL[Temporal]&= ~(3<<2*Pin);
	PINSEL[Temporal]|= (Function<<2*Pin);
}
void GPIO_Mode(uint32_t Port,uint32_t Pin, uint32_t Mode)
{
	uint32_t Temporal = Port*2;
	if(Pin>15)
	{
		Temporal++;
		Pin-=16;
	}
	PINMODE[Temporal]&= ~(3<<(2*Pin));
	PINMODE[Temporal]|= (Mode<<(2*Pin));
}

void GPIO_Dir(uint32_t Port, uint32_t Pin, uint32_t Dir)
{
	if(Dir)
		GPIOs[Port].FIODIR |= 1<<Pin;
	else
		GPIOs[Port].FIODIR |= 0<<Pin;

}
void GPIO_Set(uint32_t Port, uint32_t Pin, uint32_t Estate)
{
	if(Estate)
		GPIOs[Port].FIOSET |= 1<<Pin;
	else
		GPIOs[Port].FIOCLR |= 1<<Pin;
}

uint32_t GPIO_Get (uint32_t Port, uint32_t Pin)
{
	if(GPIOs[Port].FIOPIN & (1<<Pin))
		return 1;
	else
		return 0;
}

#if CANT_ENTRADAS_DIG>0
void GPIO_Debounce(void) //al systick
{
	uint32_t i;
	for(i=0;i<CANT_ENTRADAS_DIG;i++)
	{
		Entrada[i].Estado_actual = GPIO_Get(	Entrada[i].Port,
												Entrada[i].Pin);
		if(Entrada[i].Estado_actual == Entrada[i].Estado_anterior)
		{
			if(Entrada[i].Contador >= CANT_REBOTES)
			{
				Entrada[i].Estado_valido = Entrada[i].Estado_actual;
			}
			else
				Entrada[i].Contador++;
		}
		else
			Entrada[i].Contador=0;
		Entrada[i].Estado_anterior = Entrada[i].Estado_actual;
	}
}
int16_t GPIO_Leer_Entrada_Filtrada(uint8_t Nro)// primitiva
{
	if(Nro>=CANT_ENTRADAS_DIG)
		return -1;//error. no existe la entrada.
	return (int16_t)Entrada[Nro].Estado_valido;
}
#endif

#if CANT_SALIDAS_DIG > 0
void GPIO_Barrer_Salidas(void) // colocar en el systick
{
	uint32_t i;
	for(i=0;i<CANT_SALIDAS_DIG;i++)
	{
		if(Salida[i].Estado_valido)
			GPIO_Set(Salida[i].Port,Salida[i].Pin,1);
		else
			GPIO_Set(Salida[i].Port,Salida[i].Pin,0);
	}
}

#endif
