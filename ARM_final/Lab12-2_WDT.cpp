/*

2.只要有按著key1系統就不會Reset，沒按按鈕之後的2秒系統就會Reset。


*/

#include <stdio.h>	
#include <string.h>																										 
#include "NUC1xx.h"
#include "GPIO.h"
#include "SYS.h"
#include "LCD.h"
#include "Scankey.h"
volatile uint32_t WTDcounter = 1;  


// Watch-Dog Timer Interrupt Handler
void WDT_IRQHandler(void) 
{ 
	if(WTDcounter%5 == 0){
		print_Line(1,"WDT Interrupt");
		//	clear_LCD();		
	}	
	else{
		clear_LCD();	
	}
	WTDcounter ++;
	
	UNLOCKREG();	
	WDT->WTCR.WTIF =1;
	WDT->WTCR.WTR = 1;
	
}


// Initialize Watch-Dog Timer
void init_WDT(void)
{
	UNLOCKREG();		
	/* Enable and Select WDT clock source */         
	SYSCLK->CLKSEL1.WDT_S =3;//Select 10Khz for WDT clock source  	
	SYSCLK->APBCLK.WDT_EN =1;//Enable WDT clock source	

	/* Select Timeout Interval */
	WDT->WTCR.WTIS=4; 			
	/* Disable Watchdog Timer Reset function */
	WDT->WTCR.WTRE = 1; 											
	
	NVIC_EnableIRQ(WDT_IRQn);
	/* Enable WDT module */
	//xEnable WDT
	WDT->WTCR.WTE = 1;
	//Enable WDT INT
	WDT->WTCR.WTIE = 1;
	 //Clear WDT counter
	WDT->WTCR.WTR = 1;		
	LOCKREG();	 
}



int32_t main (void)
{
	
	int8_t number;
	
	UNLOCKREG();
	SYSCLK->PWRCON.XTL32K_EN = 1;//Enable 32KHz for RTC clock source
	SYSCLK->PWRCON.XTL12M_EN = 1;//Enable 12MHz crystal
	SYSCLK->CLKSEL0.HCLK_S = 0;
	LOCKREG();
	
	OpenKeyPad();
	
	init_LCD();
	clear_LCD();
	print_Line(0,"test");
	
	DrvGPIO_Open(E_GPC, 12, E_IO_OUTPUT); 
	DrvGPIO_ClrBit(E_GPC, 12);  
	DrvSYS_Delay(500000);	
	DrvGPIO_SetBit(E_GPC, 12);  

	init_WDT();	 	
	
	while(1){
	
		number = ScanKey();
		if(number == 1)
		{
			UNLOCKREG();	
			WDT->WTCR.WTE = 0;
			LOCKREG();
		}
		else{
			UNLOCKREG();	
			WDT->WTCR.WTE = 1;
			LOCKREG();
		
		}
	
	};
}



