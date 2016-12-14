//
// Smpl_LCD_Text: display 4 lines of Text on LCD
//
#include <stdio.h>
#include "NUC1xx.h"
#include "SYS.h"
#include "GPIO.h"
#include "LCD.h"
#include "Font5x7.h"


int main(void)
{ 
	UNLOCKREG();
	DrvSYS_Open(48000000); // set to 48MHz
	LOCKREG(); 

	init_LCD(); 
	clear_LCD();
	//     x  y  words 
	printS(0, 0,"CortexM0"); //參數: x起始bits,y起始bits,字串   y方向(行數)每行16bits寬
	printS(0, 16,"Nuvoton");
	printS(0, 32,"NUC140");
	printS(0, 48,"NTOU");    	

		
}

