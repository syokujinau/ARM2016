//
// Smpl_LCD_Text: display 4 lines of Text on LCD
//
#include <stdio.h>
#include "NUC1xx.h"
#include "SYS.h"
#include "GPIO.h"
#include "LCD.h"

void draw_rect(int x1,int y1,int x2,int y2){
	int a,b;
	for(a=x1; a<=x2; a++){
		draw_Pixel(a, y1, 1, 1);
		draw_Pixel(a, y2, 1, 1);
	}
	for(b=y1; b<=y2; b++){
		draw_Pixel(x1, b, 1, 1);
		draw_Pixel(x2, b, 1, 1);
	}
}

int32_t main (void)
{
	int a=0,b=0;
	UNLOCKREG();
	DrvSYS_SetOscCtrl(E_SYS_XTL12M,1);
  DrvSYS_Delay(20000);				          // Delay for Xtal stable
  while(!SYSCLK->CLKSTATUS.XTL12M_STB);
  DrvSYS_SelectHCLKSource(0);
  LOCKREG();

  init_LCD();  
  clear_LCD();
  
	while(1) {

		 //draw_rect(x1,y1,x2,y2); (0,0,30,30)-->(40,20,70,50)
		 
		for(b=0;b<20;b++){
			a+=2;
			draw_rect(a,b,a+30,b+30);
			DrvSYS_Delay(100000);
			clear_LCD();
			if (b==19)
			{
				a=0;
				b=0;
			}
		}
		// for(a=40;a>=0;a--){
		// 	b--;
		// 	draw_rect(a,b,a+30,b+30);
		// 	DrvSYS_Delay(100000);
		// 	clear_LCD();
		// }
//		draw_rect(0,0,30,30);
//		draw_rect(40,20,70,50);
		
	}

}



