// Output : PWM0 (GPA12)

#include <stdio.h>
#include "NUC1xx.h"
#include "GPIO.h"
#include "SYS.h"
//#include "??"
#include "Scankey.h"

int a1 = 1199;      //亮度0的值
int a2;             //暫存目前亮度

void InitPWM(void)
{
 	/* Step 1. GPIO initial */ 
	SYS->u32GPAMFP = 0x4000;	//0x1000 PWM0; 0x4000 PWM2
	//SYS->u32ALTMFP=0x????;	//set ALT_MFP->PB11_PWM4=1 to open Buzzer
	/*
	Multiple Function Pin GPIOA Control Register (GPA_MFP)
	at NuMicro NUC130/NUC140 Technical Reference Manual page 67	.
	0000 0000
	0000 0000
	0001 0000
	0000 0000
	*/		
	
	/* Step 2. Enable and Select PWM clock source*/		
	SYSCLK->APBCLK.PWM23_EN = 1;//Enable PWM clock
	SYSCLK->CLKSEL1.PWM23_S = 0;//Select 12Mhz for PWM clock source

	PWMA->PPR.CP23=1;			//Prescaler 0~255, Setting 0 to stop output clock
	PWMA->CSR.CSR2=4;			// PWM clock = clock source/(Prescaler + 1)/divider
	
	/* Step 3. Select PWM Operation mode */
	//PWM0
	PWMA->PCR.CH2MOD=1;			//0:One-shot mode, 1:Auto-load mode

	//CNR and CMR will be auto-cleared after setting CH0MOD form 0 to 1.
	PWMA->CNR2=0xFFFF;//set CNR
	PWMA->CMR2=0xFFFF;//set CNR

	PWMA->PCR.CH2INV=0;			//Inverter->0:off, 1:on
	PWMA->PCR.CH2EN=1;			//PWM function->0:Disable, 1:Enable
 	PWMA->POE.PWM2=1;			//Output to pin->0:Diasble, 1:Enable
	
}


int32_t main (void)
{
	int8_t num;  //for scankey

	UNLOCKREG();
	SYSCLK->PWRCON.XTL12M_EN = 1;		//Enable 12Mhz and set HCLK->12Mhz
	SYSCLK->CLKSEL0.HCLK_S = 0;
	LOCKREG();
	InitPWM();
	//	OpenKeyPad()
	OpenKeyPad();
  
	PWMA->CNR2 = 1199;  // set CNR 分母
	PWMA->CMR2 = 1;   // set CMR   分子 <<用這個改ratio， 100%最暗；0%最亮
	

	

	while(1){
		num = ScanKey();   //偵測key
		
		if (num == 1)
		{
			PWMA->CMR2 ++; //漸暗
			if(PWMA->CMR2 == 1198){
				PWMA->CMR2 = 1199;
			}
		}
		else if (num == 2)
		{
			PWMA->CMR2 --; //漸亮
			if(PWMA->CMR2 == 1){
				PWMA->CMR2 = 0;
			}
		}
		else if (num == 3)
		{
			DrvSYS_Delay(500000);  //防止重複偵測按鍵
			
			a2 = PWMA->CMR2;    //a2存目前亮度
			PWMA->CMR2 = a1;    //將亮度改a1(亮度0的值)
			a1 = a2;            //a1存目前亮度
			
			if(PWMA->POE.PWM2==1){   //PWM功能ON時
			  PWMA->POE.PWM2=0;      //PWM功能OFF
			}
			else {                   //否則
			  PWMA->POE.PWM2=1;      //PWM功能ON
			}
			
			
		}
			
	
	}
		
	
}


/*
PWM Frequency = PWMxy_CLK/[(prescale+1)*(clock divider)*(CNR+1)]; where xy = 01, 23, 45 or 67, the selected PWM channel.
          5kHz = 12MHz / [(1+1)*1*1199]     
Duty ratio = (CMR+1)/(CNR+1)
*/
