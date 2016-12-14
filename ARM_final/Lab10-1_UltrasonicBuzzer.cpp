//
// Smpl_LCD_Text: USE GPIO PORT
//
#include <stdio.h>
#include "NUC1xx.h"
#include "SYS.h"
#include "GPIO.h"
#include "LCD.h"

char TEXT1[16] = "";

int32_t get_Echo_Time_Ultrasonic_Ranger( E_DRVGPIO_PORT port, int32_t trig, int32_t echo ){
	uint32_t rise_us = 0, fall_us = 0, time_us = 0;
	DrvGPIO_SetBit( port, trig);
	DrvSYS_Delay(10);
	DrvGPIO_ClrBit( port, trig);
	
	while( !DrvGPIO_GetBit( port, echo)){
		DrvSYS_Delay(10);
		time_us += 20;
	}
	rise_us = time_us;
	while( DrvGPIO_GetBit( port, echo)){
		DrvSYS_Delay(10);
		time_us += 20;
	}
	fall_us = time_us;	
	
	if((fall_us - rise_us)>30000) return 0;
	
	return (fall_us - rise_us);				//RETURN TIME interval
}


int32_t main (void)
{
	int i,j;
	int dtime=0;
	DrvGPIO_Open(E_GPB,11, E_IO_OUTPUT);  //for buzzer
	UNLOCKREG();
  DrvSYS_SetOscCtrl(E_SYS_XTL12M,1);
  DrvSYS_Delay(20000);				          // Delay for Xtal stable
  while(!SYSCLK->CLKSTATUS.XTL12M_STB);
  DrvSYS_SelectHCLKSource(0);
	LOCKREG();
 
	DrvGPIO_Open( E_GPC, 10, E_IO_OUTPUT);
	DrvGPIO_Open( E_GPC, 11  , E_IO_INPUT );
	DrvGPIO_ClrBit( E_GPC, 10);
	DrvGPIO_ClrBit( E_GPC, 11);
	
	
	init_LCD();  
	clear_LCD();
    print_Line(0,"Distance(cm)=");
	while(1)
	{
		TEXT1[0] = '\0';
		                                              //tri echo
		dtime = get_Echo_Time_Ultrasonic_Ranger( E_GPC, 10, 11);
		sprintf(TEXT1,"%.2f",(dtime*0.034)/2);
		print_Line(1,TEXT1);
		DrvSYS_Delay(100000);

		if((dtime*0.034)/2 <= 10){
				DrvGPIO_ClrBit(E_GPB,11);
				DrvSYS_Delay(100000);
			  DrvGPIO_SetBit(E_GPB,11);
				DrvSYS_Delay(100000);
		
		}
		else{
			DrvGPIO_SetBit(E_GPB,11);
		  DrvSYS_Delay(100000);
		    
		}
			

	}
	
}

//(dtime*0.034)/2 (cm)



