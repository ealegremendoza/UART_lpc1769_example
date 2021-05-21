/*
 * GPIO.h
 *
 *  Created on: 19 mar. 2021
 *      Author: ealegremendoza
 */

#ifndef GPIO_H_
#define GPIO_H_
#include "headers.h"

//Estados de PINSEL
#define FUNC_GPIO	0
#define FUNC_FUNC1	1
#define FUNC_FUNC2	2
#define FUNC_FUNC3	3

//Estados de PINMODE
#define	MODE_PULLUP		0
#define	MODE_REPEAT		1
#define	MODE_NONE		2
#define	MODE_PULLDOWN	3

//Estados de FIODIR:
#define	DIR_SALIDA		SALIDA
#define DIR_ENTRADA		ENTRADA

/*	Para utilizar sensores */
#define CANT_ENTRADAS_DIG	0	//modificar esto
#if CANT_ENTRADAS_DIG >0
#define CANT_REBOTES		3
typedef struct{
	uint32_t Estado_valido;
	uint32_t Estado_actual;
	uint32_t Estado_anterior;
	uint32_t Contador;
	uint32_t Port;
	uint32_t Pin;
}STRUCT_ENTRADAS_DIGITALES;

void GPIO_Debounce(void);
int16_t GPIO_Leer_Entrada_Filtrada(uint8_t Nro);
#endif

#define CANT_SALIDAS_DIG	0	//modificar esto
#if CANT_SALIDAS_DIG >0
typedef struct{
	uint32_t Estado_valido;
	uint32_t Port;
	uint32_t Pin;
}STRUCT_SALIDAS_DIGITALES;

void GPIO_Barrer_Salidas(void);
#endif

/*	Prototipos generales	*/
void GPIO_Func(uint32_t Port, uint32_t Pin, uint32_t Function);
void GPIO_Mode(uint32_t Port,uint32_t Pin, uint32_t Mode);
void GPIO_Dir(uint32_t Port, uint32_t Pin, uint32_t Dir);
void GPIO_Set(uint32_t Port, uint32_t Pin, uint32_t Estate);
uint32_t GPIO_Get (uint32_t Port, uint32_t Pin);
#endif /* GPIO_H_ */
