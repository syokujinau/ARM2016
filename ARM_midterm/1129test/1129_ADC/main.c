// PWM4 / GPB11 : pin 48
// http://i.imgur.com/p63fMM7.png

#include <stdio.h>
#include "NUC1xx.h"
#include "GPIO.h"
#include "SYS.h"
#include "Scankey.h"
#include "LCD.h"

/****************************************************
1:Do,2:Do#,3:Re,5:Re#,6:Mi,7:Fa,8:Fa#,9:So
****************************************************/
void Tone(int x)
{
    ADC->ADCR.ADST=1;
    switch(x){
        case 1:
        print_Line(0,"Do");
        PWMB->CNR0 = 21146;
        PWMB->CMR0 = PWMB->CNR0*0.95;        
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

void delay02X(int X)
{
  int i;
  for(i=0; i<X; i++){
        DrvSYS_Delay(200000);
    }   
}

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

void InitADC(void)
{
    /* Step 1. GPIO initial */ 
    GPIOA->OFFD|=0x00800000;    //Disable digital input path
    SYS->GPAMFP.ADC7_SS21_AD6=1;        //Set ADC function 
                
    /* Step 2. Enable and Select ADC clock source, and then enable ADC module */          
    SYSCLK->CLKSEL1.ADC_S = 2;  //Select 22Mhz for ADC
    SYSCLK->CLKDIV.ADC_N = 1;   //ADC clock source = 22Mhz/2 =11Mhz;
    SYSCLK->APBCLK.ADC_EN = 1;  //Enable clock source
    ADC->ADCR.ADEN = 1;         //Enable ADC module

    /* Step 3. Select Operation mode */
    ADC->ADCR.DIFFEN = 0;       //single end input
    ADC->ADCR.ADMD   = 0;       //single mode
        
    /* Step 4. Select ADC channel */
    ADC->ADCHER.CHEN = 0x80;
    
    /* Step 5. Enable ADC interrupt */
    ADC->ADSR.ADF =1;           //clear the A/D interrupt flags for safe 
    ADC->ADCR.ADIE = 1;
    //NVIC_EnableIRQ(ADC_IRQn);
    
    /* Step 6. Enable WDT module */
    ADC->ADCR.ADST=1;
}

int32_t main (void)
{
     // char TEXT[16];
    
    UNLOCKREG();
    //SYSCLK->PWRCON.XTL12M_EN = 1;       //Enable 12Mhz and set HCLK->12Mhz
    SYSCLK->CLKSEL0.HCLK_S = 7;   //22.1185MHz
    LOCKREG();
    InitPWM();
    InitADC();          // initialize ADC
    
    init_LCD();
    print_Line(0,"OPNE LCD");
    DrvSYS_Delay(5000000);
    clear_LCD();
                    
    
    PWMB->CNR0 = 89;  // set CNR ¤À¤l
    PWMB->CMR0 = 44;   // set CMR ¤À¥À
    
    while(1)
    {
        int resistance,l;
        resistance = ADC->ADDR[7].RSLT;
        l = resistance/1000;
        Tone(1);
        delay02X(l+1);
              Tone(4);
        delay02X(l+1);
    }
}


