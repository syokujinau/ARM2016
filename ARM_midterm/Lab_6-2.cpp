// PWM4 / GPB11 : pin 48
// http://i.imgur.com/p63fMM7.png

#include <stdio.h>
#include "NUC1xx.h"
#include "GPIO.h"
#include "SYS.h"
#include "Scankey.h"
#include "LCD.h"


void InitPWM(void)
{
    SYS->u32GPBMFP = 0x0800;        //Manual pp.71.72
    SYS->ALTMFP.PB11_PWM4 = 1;
    /* Step 2. Enable and Select PWM clock source*/     
    SYSCLK->APBCLK.PWM45_EN = 1;//Enable PWM clock
    SYSCLK->CLKSEL2.PWM45_S = 3;//Clock Source Select Control Register(CLKSEL), Select 12Mhz for PWM clock source

    PWMB->PPR.CP01=1;           //PWM Pre-Scale Register (PPR),Prescaler 0~255, Setting 0 to stop output clock
    PWMB->CSR.CSR0=4;           // PWM clock = clock source/(Prescaler + 1)/divider
    
    /* Step 3. Select PWM Operation mode */
    //PWM4
    PWMB->PCR.CH0MOD=1;         //PWM Control Register  0:One-shot mode, 1:Auto-load mode
         
    //CNR and CMR will be auto-cleared after setting CH0MOD form 0 to 1.
    PWMB->CNR0=0xFFFF;
    PWMB->CMR0=0xFFFF;

    PWMB->PCR.CH0INV=0;         //Inverter->0:off, 1:on
    PWMB->PCR.CH0EN=1;          //PWM function->0:Disable, 1:Enable
    PWMB->POE.PWM0=1;           //Output to pin->0:Diasble, 1:Enable
    
}

int32_t main (void)
{
     // char TEXT[16];
    
    UNLOCKREG();
    //SYSCLK->PWRCON.XTL12M_EN = 1;       //Enable 12Mhz and set HCLK->12Mhz
    SYSCLK->CLKSEL0.HCLK_S = 7;   //22.1185MHz
    LOCKREG();
    InitPWM();
    OpenKeyPad();
    
    init_LCD();
    print_Line(0,"OPNE LCD");
    DrvSYS_Delay(5000000);
    clear_LCD();
                    
    
    PWMB->CNR0 = 89;  // set CNR ¤À¤l
    PWMB->CMR0 = 44;   // set CMR ¤À¥À
    
    while(1)
    {
        switch(ScanKey()){
            case 1:
                print_Line(0,"Do");
                PWMB->CNR0 = 21146;
                PWMB->CMR0 = PWMB->CNR0*0.95;   
//sprintf(TEXT,"POE.PWM0: %d",PWMB->POE.PWM0);      
//print_Line(1, TEXT);                      
                PWMB->POE.PWM0=1;          
                DrvSYS_Delay(10000);
                clear_LCD();
            break;

            case 2:
                print_Line(0,"Do#");
                PWMB->CNR0 = 19962;
                PWMB->CMR0 = PWMB->CNR0*0.95;            
                PWMB->POE.PWM0=1;
                DrvSYS_Delay(10000);
                clear_LCD();
            break;

            case 3:
                print_Line(0,"Re");
                PWMB->CNR0 = 18840;
                PWMB->CMR0 = PWMB->CNR0*0.95;       
                PWMB->POE.PWM0=1;
                DrvSYS_Delay(10000);
                clear_LCD();
            break;

            case 4:
                print_Line(0,"Re#");
                PWMB->CNR0 = 17780;
                PWMB->CMR0 = PWMB->CNR0*0.95;       
                PWMB->POE.PWM0=1;
                DrvSYS_Delay(10000);
                clear_LCD();
            break;

            case 5:
                print_Line(0,"Me");
                PWMB->CNR0 = 16872;
                PWMB->CMR0 = PWMB->CNR0*0.95;                      
                PWMB->POE.PWM0=1;
                DrvSYS_Delay(10000);
                clear_LCD();
            break;

            case 6:
                print_Line(0,"Fa");
                PWMB->CNR0 = 15844;
                PWMB->CMR0 = PWMB->CNR0*0.95;                       
                PWMB->POE.PWM0=1;
                DrvSYS_Delay(10000);
                clear_LCD();
            break;

            case 7:
                print_Line(0,"Fa#");
                PWMB->CNR0 = 14945;
                PWMB->CMR0 = PWMB->CNR0*0.95;                       
                PWMB->POE.PWM0=1;
                DrvSYS_Delay(10000);
                clear_LCD();
            break;

            case 8:
                print_Line(0,"So");
                PWMB->CNR0 = 14106;
                PWMB->CMR0 = PWMB->CNR0*0.95;                     
                PWMB->POE.PWM0=1;                       
                DrvSYS_Delay(10000);
                clear_LCD();
            break;

            case 9:
                print_Line(0,"So#");
                PWMB->CNR0 =13308;
                PWMB->CMR0 = PWMB->CNR0*0.95;                    
                PWMB->POE.PWM0=1;
                DrvSYS_Delay(10000);
                clear_LCD();
            break;

            default:PWMB->POE.PWM0=0;
        }
    }
}
