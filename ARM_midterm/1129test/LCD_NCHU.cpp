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


int count1=1;
int count2=2;
int count3=3;
int count4=4;
//=============================================================
// EINT1 routine
// turn on next LED in turn
//=============================================================
void EINT1Callback(void)
{
    DrvGPIO_ClrBit(E_GPA, 14);
    DrvSYS_Delay(500000);
    DrvGPIO_SetBit(E_GPA, 14);
    DrvSYS_Delay(500000);
}

//=============================================================
// MAIN function
//=============================================================

int32_t main ()
{
    char TEXT[16];
    DrvGPIO_Open(E_GPA,12,E_IO_OUTPUT);
    DrvGPIO_Open(E_GPA,13,E_IO_OUTPUT);
    DrvGPIO_Open(E_GPA,14,E_IO_OUTPUT);
    
    UNLOCKREG();
    SYSCLK->PWRCON.XTL12M_EN=1;
    DrvSYS_Delay(5000);                   // Waiting for 12M Xtal stalble
    SYSCLK->CLKSEL0.HCLK_S=0;
    LOCKREG();

    init_LCD();
    clear_LCD();
    print_Line(0,"test");
    
    
    DrvGPIO_Open(E_GPB,15,E_IO_INPUT);
    DrvGPIO_EnableInt(E_GPB, 15, E_IO_FALLING, E_MODE_EDGE);
    DrvGPIO_SetDebounceTime(5, 1);
    DrvGPIO_EnableDebounce(E_GPB, 15);
    
        
    
    DrvGPIO_EnableEINT1(E_IO_FALLING, E_MODE_LEVEL, EINT1Callback);
        
        
//=============================================================
// Main loop
//=============================================================
    /* Waiting for interrupts */
    while(1)
    {
        sprintf(TEXT,"NCHU NO%d",count1);//AA
        print_Line(0, TEXT);
        sprintf(TEXT,"NCHU NO%d",count2);//AA
        print_Line(1, TEXT);
        sprintf(TEXT,"NCHU NO%d",count3);//AA
        print_Line(2, TEXT);
        sprintf(TEXT,"NCHU NO%d",count4);//AA
        print_Line(3, TEXT);


        DrvSYS_Delay(1000000);
        
        if(count1<10)
            count1++;
        else{
            clear_LCD();
            count1=1;
        }
        if(count2<10)
            count2++;
        else{
            clear_LCD();
            count2=1;
        }
        if(count3<10)
            count3++;
        else{
            clear_LCD();
            count3=1;
        }
        if(count4<10)
            count4++;
        else{
            clear_LCD();
            count4=1;
        }
    }
}
