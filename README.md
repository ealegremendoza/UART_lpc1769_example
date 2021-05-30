#   UART0 lpc1769 Ejemplo
Proyecto ejemplo para controlar la UART0 integrada en el micro controlador LPCXpresso lpc1769.
El ejemplo envía datos generados por el systick cada 1seg por UART0. 

- IDE: [MCUXpresso IDE](https://www.nxp.com/design/software/development-software/mcuxpresso-software-and-tools-/mcuxpresso-integrated-development-environment-ide:MCUXpresso-IDE).
- Lenguaje: C.
- Microcontrolador: [LPCXpresso lpc1769 rev C](https://www.embeddedartists.com/products/lpc1769-lpcxpresso/).
- UART0 1-N-8
- Systick 1000Hz 

---
## LPCXpresso LPC1769
![lpc1769](img/lpc1769_lpcxpresso.png "LPCXpresso LPC1769 revC.") 
![lpc1769_pinout](img/lpc1769_pinout.png "LPCXpresso LPC1769 pinout")

---
### PCONP
![lpc1769_PCONP](img/PCONP.png "LPC1769 PCONP REGISTER")

---
### PCLKSELx
![lpc1769_PCLKSEL](img/PCLKSEL.png "LPC1769 PCLKSELx REGISTER")

---
### ISER0
![lpc1769_ISER0](img/ISER0.png "LPC1769 ISER0 REGISTER")

---
### UART0 - REGISTROS
![lpc1769_UART0_Regs](img/UART_reg.png "LPC1769 UART0 REGISTERS")
![lpc1769_UART0_Regs](img/UART_reg_utn.png "LPC1769 UART0 REGISTERS")

---
### TESTING
![lpc1769_UART0_PC_testing](img/lpc1769_arduino_bridgeUSB.png "Circuito de prueba para testear el funcionamiento del código.")
![lpc1769_UART0_PC_testing_example](img/example.png "Puesta a prueba del codigo. Se utiliza el programa Putty para leer los bytes recibidos por puerto serie")
---

Espero le sea de útilidad.

Aportes y sugerencias siempre serán bienvenidas.

Autor: @ealegremendoza

E-mail: ealegremendoza@gmail.com
