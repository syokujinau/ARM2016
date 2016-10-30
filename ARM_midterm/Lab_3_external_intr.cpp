//
// smpl_GPIO_Interrupt
//
// GPA15 to input interrupt
// GPE15 to input interrupt

#include <stdio.h>
#include "NUC1xx.h"
#include "UART.h"
#include "GPIO.h"
#include "SYS.h"
#include "LCD.h"


//=============================================================
// EINT1 routine
// turn on next LED in turn
//=============================================================
void EINT1Callback(void)
{


}


//=============================================================
// MAIN function
//=============================================================

int main (void)
{
  UNLOCKREG();
  SYSCLK->PWRCON.XTL12M_EN=1;
  DrvSYS_Delay(5000);                   // Waiting for 12M Xtal stalble
  SYSCLK->CLKSEL0.HCLK_S=0;
  LOCKREG();


    
    
    
        
    
  DrvGPIO_EnableEINT1(E_IO_FALLING, E_MODE_EDGE, EINT1Callback);
        
        
//=============================================================
// Main loop
//=============================================================
    /* Waiting for interrupts */
    while(1)
    {
        
        
    }
}

/*****************************************************
外部中斷設定步驟
Step1(44):該函式專門用於監看GPB15按鍵，設定觸發條件(triggerType、triggerMode)，EINT1Callback為中斷副函式(ISR)
*****************************************************/

