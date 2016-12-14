//
// Smpl_Timer_LED : on/off LED every second
//
#include <stdio.h>
#include <stdbool.h>
#include "NUC1xx.h"
#include "SYS.h"
#include "GPIO.h"
#include "Seven_Segment.h"
//include "XXXX.h"

#define  ONESHOT  0   // counting and interrupt when reach TCMPR number, then stop
#define  PERIODIC 1   // counting and interrupt when reach TCMPR number, then counting from 0 again
#define  TOGGLE   2   // keep counting and interrupt when reach TCMPR number, tout toggled (between 0 and 1)
#define  CONTINUOUS 3 // keep counting and interrupt when reach TCMPR number

volatile uint32_t  ledState = 0;
volatile uint32_t  count = 0;


  
void TMR0_IRQHandler()//XXX
{
	
	DrvGPIO_ClrBit(E_GPA,13);   //green RGB
	TIMER0->TISR.TIF = 1;		// clear Interrupt flag
}


void TMR1_IRQHandler()//XXX
{
	
	if (count >= 2)
	{
		ledState = ~ledState;  // changing ON/OFF state  EX: A=10011  ~A=01100
	    if(ledState) DrvGPIO_ClrBit(E_GPC,15); //on
	    else         DrvGPIO_SetBit(E_GPC,15);
	}
	
	count ++;
	TIMER1->TISR.TIF = 1;		// clear Interrupt flag
}

void InitTIMER0(void)
{
	/* Step 1. Enable and Select Timer clock source */          
	SYSCLK->CLKSEL1.TMR0_S = 0;	//Select 0(12Mhz)  for Timer0 clock source ---AAA--- 1(32.768kHz)
	SYSCLK->APBCLK.TMR0_EN = 1;	//Enable Timer0 clock source

	/* Step 2. Select Operation mode */	
	TIMER0->TCSR.MODE = ONESHOT; //Select operation mode 

	/* Step 3. Select Time out period = (Period of timer clock input) * (8-bit Prescale + 1) * (24-bit TCMP)*/
	TIMER0->TCSR.PRESCALE = 255;	// Set Prescale [0~255]	---BBB---
	TIMER0->TCMPR = 140625;		    // Set TCMPR [0~16777215] ---CCC---
	//Timeout period = (1/AAA)*(BBB+1)* CCC = Y sec  (1/10M)*(255+1)*140625 = 3s

	/* Step 4. Enable interrupt */
	TIMER0->TCSR.IE = 1;		//Enable timer Interupt
	TIMER0->TISR.TIF = 1;		//Write 1 to clear for safty		
	NVIC_EnableIRQ(TMR0_IRQn);	//Enable Timer0 Interrupt

	/* Step 5. Enable Timer module */
	TIMER0->TCSR.CRST = 1;	//Reset up counter
	TIMER0->TCSR.CEN = 1;		//Enable Timer0

}

void InitTIMER1(void){
	/* Step 1. Enable and Select Timer clock source */          
	SYSCLK->CLKSEL1.TMR1_S = 0;	//Select 0(12Mhz)  for Timer1 clock source ---AAA--- 1(32.768kHz)
	SYSCLK->APBCLK.TMR1_EN = 1;	//Enable Timer1 clock source

	/* Step 2. Select Operation mode */	
	TIMER1->TCSR.MODE = PERIODIC; //Select operation mode 

	/* Step 3. Select Time out period = (Period of timer clock input) * (8-bit Prescale + 1) * (24-bit TCMP)*/
	TIMER1->TCSR.PRESCALE = 255;	// Set Prescale [0~255]	---BBB---
	TIMER1->TCMPR = 46875;		    // Set TCMPR [0~16777215] ---CCC---
	//Timeout period = (1/AAA)*(BBB+1)* CCC = Y sec  (1/12M)*(255+1)*46875 = 1s

	/* Step 4. Enable interrupt */
	TIMER1->TCSR.IE = 1;		//Enable timer Interupt
	TIMER1->TISR.TIF = 1;		//Write 1 to clear for safty		
	NVIC_EnableIRQ(TMR1_IRQn);	//Enable Timer1 Interrupt

	/* Step 5. Enable Timer module */
	TIMER1->TCSR.CRST = 1;	//Reset up counter
	TIMER1->TCSR.CEN = 1;		//Enable Timer0
}



int32_t main(void)
{
	DrvGPIO_Open(E_GPC,15, E_IO_OUTPUT); // set GPC12 output for LED
	DrvGPIO_Open(E_GPA,13, E_IO_OUTPUT); 
	UNLOCKREG();
	SYSCLK->PWRCON.XTL12M_EN = 1;//Enable 12MHz oscillator 
    DrvSYS_Delay(5000); // waiting for 12MHz crystal stable
	SYSCLK->CLKSEL0.HCLK_S = 0;		
	LOCKREG();
	
	InitTIMER0();                        // Set Timer0 Ticking
	InitTIMER1();                        // Set TimerX Ticking
	while(1){
	};  // Do Nothing

}

/********************************************
初始化Timer Intr步驟
Step1:Line99 改頻率，同時75行也須更改
Step2:Line47 選擇MODE，改TMR0_S (p.4) 0為12MHz範圍
Step3:Line51 TCSR.MODE選MODE
Step4:Line55 代公式求TCMPR，根據想要的秒數，用公式Timeout period = (1/AAA)*(BBB+1)* CCC = Y sec  求出CCC之值
********************************************/
