//
// Smpl_UART0 : while loop for UART0-TX keep transmitting 8 bytes string
//            : IRQ routine for UART0-RX keep receiving 8 bytes string & print to LCD
//            : need two learning board to perform UART communication (TX & RX at the same time)
//
// Nu-LB-NUC140
// pin32 GPB0/RX0 to another board's UART TX
// pin33 GPB1/TX0 to another board's UART RX

#include <stdio.h>
#include "NUC1xx.h"
#include "GPIO.h"
#include "SYS.h"
#include "UART.h"
#include "LCD.h"

volatile uint8_t  comRbuf;
volatile uint16_t comRbytes = 0;
volatile uint16_t comRhead 	= 0;
volatile uint16_t comRtail 	= 0;

char TEXT[14] = "Receive:     ";
UART_T * tUART;

/*---------------------------------------------------------------------------------------------------------*/
/* UART Callback function                                                                           	   */
/*---------------------------------------------------------------------------------------------------------*/
void UART_INT_HANDLE(void)
{
	uint8_t bInChar[1] = {0xFF};

	while(UART0->ISR.RDA_IF==1) 
	{
		DrvUART_Read(UART_PORT0,bInChar,1);			
		comRbuf  = bInChar[0];
		sprintf(TEXT+9,"%c",comRbuf);
		print_Line(1,TEXT);				
	}	   
}


int32_t main()
{
	uint8_t  i =0;
	uint8_t  dataout[4] = "ABCD";
	STR_UART_T sParam;

	UNLOCKREG();
	DrvSYS_Open(50000000);
	LOCKREG();
	
	init_LCD();
	clear_LCD();
	print_Line(0,"Smpl_UART0    ");
   	
	/* Set UART Pin */
	DrvGPIO_InitFunction(E_FUNC_UART0);		

	/* UART Setting */
    sParam.u32BaudRate 		= 9600;
    sParam.u8cDataBits 		= DRVUART_DATABITS_8;
    sParam.u8cStopBits 		= DRVUART_STOPBITS_1;
    sParam.u8cParity 		= DRVUART_PARITY_NONE;
    sParam.u8cRxTriggerLevel= DRVUART_FIFO_1BYTES;

	/* Set UART Configuration */
 	if(DrvUART_Open(UART_PORT0,&sParam) != E_SUCCESS);  

	DrvUART_EnableInt(UART_PORT0, DRVUART_RDAINT, UART_INT_HANDLE);  
	
	tUART = (UART_T *)((uint32_t)UART0 + UART_PORT0); 
	
	while(1)
	{
		//tUART->DATA = dataout[i];
		if(UART0->ISR.RDA_IF==1){
			tUART->DATA = comRbuf;
		}
//		i++;
//		i=i%4;	
		DrvSYS_Delay(300000);
	}
}

