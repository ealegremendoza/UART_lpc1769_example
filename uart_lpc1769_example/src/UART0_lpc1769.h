/*
 * UART_lpc1769.h
 *
 *  Created on: 21 may. 2021
 *      Author: eze
 */

#ifndef UART0_LPC1769_H_
#define UART0_LPC1769_H_

#include "headers.h"
#define UART0_RX_PIN PORT_UART0_RX_PIN
#define UART0_TX_PIN PORT_UART0_TX_PIN
#define PORT_UART0_FUNC 	1
#define CORE_CLK 100000000

#define TAMANIO_BUFFER_TX 	50
#define TAMANIO_BUFFER_RX 	50
#define	TAMANIO_TRAMA_TX	50

void UART0_Init(uint32_t baudrate);
void UART0_IRQHandler(void);
int16_t Pop_Tx(void);
void UART0_TX(void);
void UART0_TX_Byte(uint8_t Dato);
void UART0_TX_Datos(uint8_t *Datos, uint32_t tamanio);
void UART0_RX(void);
void Push_Rx(uint8_t Dato);
int16_t Pop_Rx(void);
void MdE_Rx(uint8_t Dato);
void UART0_TX_Armar_Trama(void);

#endif /* UART0_LPC1769_H_ */
