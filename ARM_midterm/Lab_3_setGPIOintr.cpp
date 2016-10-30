//
// smpl_GPIO_Interrupt    GPIO中斷
//
// GPA15 to input interrupt
// GPE15 to input interrupt

#include <stdio.h>
#include "NUC1xx.h"
#include "UART.h"
#include "GPIO.h"
#include "SYS.h"
#include "LCD.h"

//宣告變數:中斷發生時使該變數+1， 作為計數功能
volatile uint32_t irqA_counter = 0;    
volatile uint32_t irqE_counter = 0;

void GPIOAB_INT_CallBack(uint32_t GPA_IntStatus, uint32_t GPB_IntStatus)
{
     if ((GPA_IntStatus>>13) & 0x01) irqA_counter++;//GPA_IntStatus>>AA
     print_Line(3,"GPA interrupt !!");
}

void GPIOCDE_INT_CallBack(uint32_t GPC_IntStatus, uint32_t GPD_IntStatus, uint32_t GPE_IntStatus)
{  
     if ((GPE_IntStatus>>15) & 0x01) irqE_counter++;//GPA_IntStatus>>BB
     print_Line(3,"GPE interrupt !!");
}

int32_t main()
{
    char TEXT[16];                       //宣告一陣列儲存LCD顯示文字

    UNLOCKREG();               
    SYSCLK->PWRCON.XTL12M_EN=1;         // 
    DrvSYS_Delay(5000);                 // Waiting for 12M Xtal stalble
    SYSCLK->CLKSEL0.HCLK_S=0;
    LOCKREG();

  // setup GPA13 & GPE15 to get interrupt input
    DrvGPIO_Open(E_GPA,13,E_IO_INPUT);//AA      
    DrvGPIO_Open(E_GPE,15,E_IO_INPUT);//BB
    DrvGPIO_EnableInt(E_GPA, 13, E_IO_RISING, E_MODE_EDGE);//AA  
    DrvGPIO_EnableInt(E_GPE, 15, E_IO_RISING, E_MODE_EDGE);//BB
    DrvGPIO_SetDebounceTime(5, 1);             //參數用於設定取樣頻率 (見圖01.jpg)
    DrvGPIO_EnableDebounce(E_GPA, 13);//AA
    DrvGPIO_EnableDebounce(E_GPE, 15);//BB  
    
    DrvGPIO_SetIntCallback(GPIOAB_INT_CallBack, GPIOCDE_INT_CallBack);

    init_LCD();
    clear_LCD();
                    
    print_Line(0,"Smpl_GPIO_Intr");
    
    while(1)
    {
        sprintf(TEXT,"IRQ_A: %d",irqA_counter);//AA
        print_Line(1, TEXT);
        sprintf(TEXT,"IRQ_E: %d",irqE_counter);//BB
        print_Line(2, TEXT);
    }
}

/*******************************************
GPIO中斷 設定步驟
Step1(41):開啟"觸發中斷"所使用之Pin腳，參數:
Step2(43):Enable中斷，參數用於設定該Pin腳 TriggerType、TriggerMode，即為GPIO中斷觸發的"條件"
Step3(45):設定debounce，避免重複觸發同一個條件
Step4(46):開啟debounce
Step5(49):初始化callback函式
Step6(18):修改群組AB或群組CDE的callback函式，其中修改GPx_IntStatus>>y(x為群組，y為pin號)
*******************************************/

