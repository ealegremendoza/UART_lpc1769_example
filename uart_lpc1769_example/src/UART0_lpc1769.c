/*
 * UART_lpc1769.C
 *
 *  Created on: 21 may. 2021
 *      Author: eze
 */

#include "UART0_lpc1769.h"


uint8_t Buffer_Tx[TAMANIO_BUFFER_TX];
uint8_t IndiceTxIn=0;
uint8_t IndiceTxOut=0;
uint8_t Flag_Tx_en_curso=0;

uint8_t Buffer_Rx[TAMANIO_BUFFER_RX];
uint8_t IndiceRxIn=0;
uint8_t IndiceRxOut=0;

/*	@brief:		Inicialización del periférico UART0
 * 	@param:		Nada.
 * 	@return:	Nada.
 * */
void UART0_Init(uint32_t baudrate)
{
	uint32_t Temporal=0;
	PCONP|=(1<<3); 			// ENCIENDO EL MODULO UART0
	PCLKSEL0 &= ~(3<<6);	// Defino CCLK/4 como clock del periferico

	/* PAGINACIÓN
	 * Para acceder a los registros DLM y DLL hay que poner el registro LCR en 0x80.
	 * Esto implica poner el bit DLAB en 1.
	 * Seguidamente se debe acceder al modo de configuración de la comunicación.
	 * Se pone DLAB en 0.
	 *  */
	U0LCR|=(1<<7);//DLAB=1, PARA ACCEDER A DLM Y DLL
	Temporal = (CORE_CLK/4)/(16*baudrate);
	U0DLM = (uint8_t) (Temporal>>8);
	U0DLL = (uint8_t) (Temporal);

	/* Configuro los pines con la función UART0.*/
	GPIO_Func(UART0_TX_PIN,1);
	GPIO_Func(UART0_RX_PIN,1);

	U0LCR &=~(0x80);// DELAB=0 para habilitar las interrupciones
	U0IER|= (0x03); // Habilitamos las interrupciones de RX y TX
	U0LCR|=(0x03);	// 8-N-1

	ISER0|=(1<<5);	// Habilito la INT de la UART0 del lado del micro
}

/*	@brief:		Rutina que atiende la interrupcion de la UART0
 * 	@param:		Nada.
 * 	@return:	Nada.
 * 	@note:		Entra a esta funcion cuando llega un dato RX o cuando
 * 				se termina de enviar un dato TX.
 * El registro IIR identifica la interrupcion -> x x 0 0 x b2 b1 b0
 * b2	b1
 * 1	0	->	dato disponible(RX)
 * 0	1	-> 	THR disponible(TX)
 * --------------------------------
 * b0
 * 0 ->	interrupcion TX/RX disponible
 * 1 ->	interrupcion no disponible
 * */
void UART0_IRQHandler(void)
{
	uint8_t Temporal;
	do{
		Temporal=U0IIR; //IIR bits -> x x 0 0 x b2 b1 b0
		switch(Temporal>>1)//desplazo 1 por el b0
		{
			case 0x02://RX
				UART0_RX();
				break;
			case 0x01://TX
				UART0_TX();
				DEBUGOUT("TX\n");
				break;
			default:
				break;
		}

	}while(0==(Temporal&1));
}

/*	@brief:		Obtiene un dato(1byte) del buffer a transmitir.
 * 	@param:		Nada.
 * 	@return:	Dato por exito.
 * 				-1 por error. El buffer está vacío.
 * 	*/
int16_t Pop_Tx(void)
{
	uint8_t Dato;
	if(IndiceTxIn == IndiceTxOut)
	{
		//buffer vacio
		return (int16_t)-1;
	}
	Dato=Buffer_Tx[IndiceTxOut];
	IndiceTxOut++;
	IndiceTxOut%= TAMANIO_BUFFER_TX;
	return (int16_t) Dato;
}

/*	@brief:		Carga un dato(1byte) al buffer a transmitir.
 * 	@param:		Nada.
 * 	@return:	Nada.
 * 	*/
void UART0_TX_Byte(uint8_t Dato)
{
	Buffer_Tx[IndiceTxIn]=Dato;
	IndiceTxIn++;
	IndiceTxIn%=TAMANIO_BUFFER_TX;
	/*	Si no hay tx en curso debo forzar una Tx para que
	 * se active la interrupcion que llama a UART0_TX()
	 * */
	if(0==Flag_Tx_en_curso){
		U0THR=(uint8_t)Pop_Tx();
		Flag_Tx_en_curso=1;
	}
}

/*	@brief:		Envía un dato(1byte) del buffer a transmitir por UART0.
 * 	@param:		Nada. El dato a enviar proviene de Pop_Tx().
 * 	@return:	Nada.
 * 	@note:		Esta función es llamada desde la rutina que
 * 				atiende la interrupción.
 * 	*/
void UART0_TX(void)
{
	int16_t Dato=Pop_Tx();
	if(Dato >= 0)
	{
		U0THR=(uint8_t)Dato;
	}
	else
		Flag_Tx_en_curso=0;//ya no hay nada en el buffer
}


/*	@brief:		Realiza la carga de una cadena de datos (1byte/dato) en
 * 				el buffer.
 * 	@param:		*Datos.		Vector de datos a enviar.
 * 	@param:		tamanio.	Tamaño del Vector de datos a enviar.
 * 	@return:	Nada.
 * 	@note:		Esta función es la que se utiliza para enviar una trama.
 * 				Si se envía 0, indica que es una cadena de caracteres (string)
 * 				y calcula su tamanio con la función strlen().
 * 	*/
void UART0_TX_Datos(uint8_t *Datos, uint32_t tamanio)
{
	uint32_t i;
	if(0==tamanio)
		tamanio=strlen((char*)Datos);
	for(i=0;i<tamanio;i++)
	{
		UART0_TX_Byte(Datos[i]);
	}
}

/*	@brief:		Realiza la lectura del dato recibido y lo carga en un buffer RX.
 * 	@param:		Nada.
 * 	@return:	Nada.
 * 	@note:		Esta función es llamada desde la rutina que
 * 				atiende la interrupción.
 * 	*/
void UART0_RX(void)
{
	uint32_t Temporal=U0RBR;
	Push_Rx(Temporal);
}

/*	@brief:		Carga un dato(1byte) al buffer de datos recibidos.
 * 	@param:		Dato.	Byte de datos recibidos.
 * 	@return:	Nada.
 * 	*/
void Push_Rx(uint8_t Dato)
{
	Buffer_Rx[IndiceRxIn]=Dato;
	IndiceRxIn++;
	IndiceRxIn%=TAMANIO_BUFFER_RX;
}

/*	@brief:		Obtiene un dato(1byte) del buffer de datos recibidos.
 * 	@param:		Nada.
 * 	@return:	Dato por exito.
 * 				-1 por error. El buffer está vacío.
 * 	*/
int16_t Pop_Rx(void)
{
	int16_t Dato=-1;
	if(IndiceRxOut != IndiceRxIn)// Buffer no vacio
	{
		Dato=(int16_t)Buffer_Rx[IndiceRxOut];
		IndiceRxOut++;
		IndiceRxOut%=TAMANIO_BUFFER_RX;
	}
	return Dato;
}

/*	@brief: 	Ejemplo de analisis de la trama recibida
 *  @param:		Dato.	Bytes de datos recibido.
 *  @return:	Nada.
 *  @note: 		La trama es: >t1=123,t2=100,chk<
 * */
void MdE_Rx(uint8_t Dato)
{
	static uint8_t MdE_Estado_Rx=0;
	static uint32_t T1=0;
	static uint32_t T2=0;
	static uint32_t chk=0;
	switch(MdE_Estado_Rx)
	{
		case 0:
			if(Dato=='>')
			{
				chk=Dato;
				MdE_Estado_Rx=1;
			}
			else
				MdE_Estado_Rx=0;
			break;
		case 1:
			if(Dato=='t')
			{
				chk+=Dato;
				MdE_Estado_Rx=2;
			}
			else
				MdE_Estado_Rx=0;
			break;
		case 2:
			if(Dato=='1')
			{
				chk+=Dato;
				MdE_Estado_Rx=3;
			}
			else
				MdE_Estado_Rx=0;
			break;
		case 3:
			if(Dato=='=')
			{
				chk+=Dato;
				MdE_Estado_Rx=4;
			}
			else
				MdE_Estado_Rx=0;
			break;
		case 4:
			chk+=Dato;
			if(Dato==',')
			{
				MdE_Estado_Rx=5;
			}
			else
			{
				T1*=10;
				T1+=Dato-'0';
				MdE_Estado_Rx=4;
			}
			break;
		case 5:
			if(Dato=='t')
			{
				chk+=Dato;
				MdE_Estado_Rx=6;
			}
			else
				MdE_Estado_Rx=0;
			break;
		case 6:
			if(Dato=='2')
			{
				chk+=Dato;
				MdE_Estado_Rx=7;
			}
			else
				MdE_Estado_Rx=0;
			break;
		case 7:
			if(Dato=='=')
			{
				chk+=Dato;
				MdE_Estado_Rx=8;
			}
			else
				MdE_Estado_Rx=0;
			break;
		case 8:
			chk+=Dato;
			if(Dato==',')
			{
				MdE_Estado_Rx=9;
			}
			else
			{
				T2*=10;
				T2+=Dato-'0';
				MdE_Estado_Rx=8;
			}
			break;
		case 9:
			if(chk==Dato)//verifico el checksum
			{
				/* Si la trama llegó correctamente escribro en las
				 * variables globales.
				 * ejemplo:
				 * Temperatura_1 = T1;
				 * Temperatura_2 = T2;
				*/
			}
			MdE_Estado_Rx=0;
		default:
			break;
	}
}


/*	@brief: 	Ejemplo de armado de trama para enviar por UART0.
 *  @param:		Nada. Los datos estarían en buffers globales.
 *  @return:	Nada.
 *  @note: 		La trama es: >t1=123,t2=100,chk<
 * */
void UART0_TX_Armar_Trama(void)
{
	//uint32_t i;
	char trama[TAMANIO_TRAMA_TX];
	/*sprtinf(trama,">t1=%03d,t2=%03d,",Temperatura_1,Temperatura2);
	for(i=0;i<15;i++)
	{
		chk+=trama[i];
	}
	trama[15]=chk;
	trama[16]='<';
	trama[17]='\0';*/
	sprintf(trama,"hola\n\r");
	UART0_TX_Datos((uint8_t*)trama,0);
}
