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

volatile uint32_t ledState = 0;
volatile uint32_t count = 0;

  
void TMR0_IRQHandler()//XXX
{
	unsigned char a[]={0,1,2,3,4,5,6,7,8,9};
	
	show_seven_segment(0,a[count]);
	
    count++;
	if(count==10) count=0;
//	ledState = ~ledState;  // changing ON/OFF state  EX: A=10011  ~A=01100
//	if(ledState) DrvGPIO_ClrBit(E_GPC,12);
//	else         DrvGPIO_SetBit(E_GPC,12);
	TIMER0->TISR.TIF = 1;		// clear Interrupt flag
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
	TIMER0->TCMPR = 64;		    // Set TCMPR [0~16777215] ---CCC---
	//Timeout period = (1/AAA)*(BBB+1)* CCC = Y sec  (1/32.768k)*(255+1)*64 = 500ms(<---看要多少)

	/* Step 4. Enable interrupt */
	TIMER0->TCSR.IE = 1;		//Enable timer Interupt
	TIMER0->TISR.TIF = 1;		//Write 1 to clear for safty		
	NVIC_EnableIRQ(TMR0_IRQn);	//Enable Timer0 Interrupt

	/* Step 5. Enable Timer module */
	TIMER0->TCSR.CRST = 1;	//Reset up counter
	TIMER0->TCSR.CEN = 1;		//Enable Timer0

}
/*
void InitTIMERX(void){
X
XX
XXX
XXXX
XXXXX
}
*/


int32_t main(void)
{
	// DrvGPIO_Open(E_GPC,12, E_IO_OUTPUT); // set GPC12 output for LED
	UNLOCKREG();
	SYSCLK->PWRCON.XTL32K_EN = 1;//Enable 32MHz oscillator 
    DrvSYS_Delay(5000); // waiting for 12MHz crystal stable
	SYSCLK->CLKSEL0.HCLK_S = 0;		
	LOCKREG();
	
	InitTIMER0();                        // Set Timer0 Ticking
	//InitTIMERX();                        // Set TimerX Ticking
	while(1){
	   close_seven_segment();
	};  // Do Nothing

}

/*****************************************************
Timer中斷設定步驟 功能:開機後某個時間間隔做某件事
Step1(39):選擇頻率，同時75行也須更改
Step2(43):選擇MODE
Step3(47):根據想要的秒數，用公式Timeout period = (1/AAA)*(BBB+1)* CCC = Y sec  求出CCC之值
Time out period = 
(Period of timer clock input) * (8-bit Prescale + 1) * (24-bit TCMPR) 

*****************************************************/
