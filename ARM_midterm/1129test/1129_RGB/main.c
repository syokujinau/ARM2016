/******************************************************************************
Behavior
RGB                  |--G--|--Y--|-----R-----|
7-Segment start|--0--|--1--|--2--|--3--|--4--|.
*****************************************************************************/
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

volatile uint32_t counter = 0;
volatile uint32_t TimerCounter = 0;
unsigned char a[]={0,1,2,3,4};
  
void TMR0_IRQHandler()//XXX
{
	
	TimerCounter == 4 ? (TimerCounter = 1) : (TimerCounter++);

	TIMER0->TISR.TIF = 1;		// clear Interrupt flag
}

void TMR1_IRQHandler()//XXX
{
	counter++;
	switch (counter%4)
        {
            case 1:
            DrvGPIO_SetBit(E_GPA,12); //B
						DrvGPIO_ClrBit(E_GPA,13); //G
						DrvGPIO_SetBit(E_GPA,14); //R
            break;
            case 2:
            DrvGPIO_SetBit(E_GPA,12);
						DrvGPIO_ClrBit(E_GPA,13);
						DrvGPIO_ClrBit(E_GPA,14);
            break;
            case 3:
            DrvGPIO_SetBit(E_GPA,12);
						DrvGPIO_SetBit(E_GPA,13);
						DrvGPIO_ClrBit(E_GPA,14);
            break;
            case 0:
            DrvGPIO_SetBit(E_GPA,12);
						DrvGPIO_SetBit(E_GPA,13);
						DrvGPIO_ClrBit(E_GPA,14);
            break;

            default:
            DrvGPIO_SetBit(E_GPA,12);
						DrvGPIO_SetBit(E_GPA,13);
						DrvGPIO_SetBit(E_GPA,14);
            break;
        }

	TIMER1->TISR.TIF = 1;		// clear Interrupt flag
}


void InitTIMER0(void)
{
	/* Step 1. Enable and Select Timer clock source */          
	SYSCLK->CLKSEL1.TMR0_S = 1;	//Select 1(32.768kHz)  for Timer0 clock source ---AAA---       (p.4)
	SYSCLK->APBCLK.TMR0_EN = 1;	//Enable Timer0 clock source

	/* Step 2. Select Operation mode */	
	TIMER0->TCSR.MODE = PERIODIC; //Select operation mode                                      (p.3)

	/* Step 3. Select Time out period = (Period of timer clock input) * (8-bit Prescale + 1) * (24-bit TCMP)*/
	TIMER0->TCSR.PRESCALE = 255;	// Set Prescale [0~255]	---BBB---
	TIMER0->TCMPR = 128;		    // Set TCMPR [0~16777215] ---CCC---
	//Timeout period = (1/AAA)*(BBB+1)* CCC = Y sec  (1/32.768k)*(255+1)*128 = 1s(<---????)

	/* Step 4. Enable interrupt */
	TIMER0->TCSR.IE = 1;		//Enable timer Interupt
	TIMER0->TISR.TIF = 1;		//Write 1 to clear for safty		
	NVIC_EnableIRQ(TMR0_IRQn);	//Enable Timer0 Interrupt

	/* Step 5. Enable Timer module */
	TIMER0->TCSR.CRST = 1;	//Reset up counter
	TIMER0->TCSR.CEN = 1;		//Enable Timer0
}

void InitTIMER1(void)
{
	/* Step 1. Enable and Select Timer clock source */          
	SYSCLK->CLKSEL1.TMR1_S = 1;	//Select 1(32.768kHz)  for Timer0 clock source ---AAA---       (p.4)
	SYSCLK->APBCLK.TMR1_EN = 1;	//Enable Timer0 clock source

	/* Step 2. Select Operation mode */	
	TIMER1->TCSR.MODE = PERIODIC; //Select operation mode                                      (p.3)

	/* Step 3. Select Time out period = (Period of timer clock input) * (8-bit Prescale + 1) * (24-bit TCMP)*/
	TIMER1->TCSR.PRESCALE = 255;	// Set Prescale [0~255]	---BBB---
	TIMER1->TCMPR = 128;		    // Set TCMPR [0~16777215] ---CCC---
	//Timeout period = (1/AAA)*(BBB+1)* CCC = Y sec  (1/32.768k)*(255+1)*128 = 1s(<---????)

	/* Step 4. Enable interrupt */
	TIMER1->TCSR.IE = 1;		//Enable timer Interupt
	TIMER1->TISR.TIF = 1;		//Write 1 to clear for safty		
	NVIC_EnableIRQ(TMR1_IRQn);	//Enable Timer0 Interrupt

	/* Step 5. Enable Timer module */
	TIMER1->TCSR.CRST = 1;	//Reset up counter
	TIMER1->TCSR.CEN = 1;		//Enable Timer0
}



int32_t main(void)
{
	DrvGPIO_Open(E_GPA,14, E_IO_OUTPUT);  //R
	DrvGPIO_Open(E_GPA,13, E_IO_OUTPUT);  //G
	DrvGPIO_Open(E_GPA,12, E_IO_OUTPUT);  //B
	// DrvGPIO_Open(E_GPC,12, E_IO_OUTPUT); // set GPC12 output for LED
	UNLOCKREG();
	SYSCLK->PWRCON.XTL32K_EN = 1;//Enable 32MHz oscillator 
  DrvSYS_Delay(5000); // waiting for 12MHz crystal stable
	SYSCLK->CLKSEL0.HCLK_S = 0;		
	LOCKREG();
	
	InitTIMER0();                        // Set Timer0 Ticking
	InitTIMER1();                        // Set Timer1 Ticking
	while(1){
	   close_seven_segment();
	   show_seven_segment(0,a[TimerCounter]);
		 DrvSYS_Delay(200);
	};  // Do Nothing

}


