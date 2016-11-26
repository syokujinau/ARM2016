/******************************************************************************
RGB                  |-----|-----|-----Y-----|--R--|--B--|
7-Segment start|--0--|--1--|--2--|--3--|--4--|--5--|--6--|
                     |--7--|--8--|--9--|--10-|--11-|--12-|
                     |--13-|--14-|--15-|--16-|--17-|--18-|
                     |--19-|--20-|--21-|--22-|--23-|--24-|...
*****************************************************************************/
#include <stdio.h>
#include "M451Series.h"
#include "NuEdu-Basic01.h"
uint32_t volatile counter = 0;
/*---------------------------------------------------------------------------------------------------------*/
/*  TMR0 IRQ handler                                                                                       */
/*---------------------------------------------------------------------------------------------------------*/
uint32_t volatile TimerCounter = 0;
void TMR0_IRQHandler(void)
{
    TimerCounter == 99 ? (TimerCounter = 0) : (TimerCounter++);
    // clear Timer0 interrupt flag
    TIMER_ClearIntFlag(TIMER0);
}

void TMR1_IRQHandler(void)
{
        counter++;
        switch (counter%6)
        {
            case 3:
            PC9  = 0; 
            PC10 = 0;
            PC11 = 1;
            break;
            case 4:
            PC9  = 0; 
            PC10 = 0;
            PC11 = 1;
            break;
            case 5:
            PC9  = 0; 
            PC10 = 1;
            PC11 = 1;
            break;
            case 0:
            PC9  = 1; 
            PC10 = 1;
            PC11 = 0;
            break;

            default:
            PC9  = 1; 
            PC10 = 1;
            PC11 = 1;
            break;
        }
    TIMER_ClearIntFlag(TIMER1);
}

/*---------------------------------------------------------------------------------------------------------*/
/*  MAIN function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int main(void)
{
    //Initial System
    SYS_Init();
    GPIO_SetMode(PC, BIT9, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PC, BIT10, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PC, BIT11, GPIO_MODE_OUTPUT);

    //Enable Timer0 clock and select Timer0 clock source
    CLK_EnableModuleClock(TMR0_MODULE);
    CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0SEL_HXT, 0);
        CLK_EnableModuleClock(TMR1_MODULE);
    CLK_SetModuleClock(TMR1_MODULE, CLK_CLKSEL1_TMR1SEL_HXT, 0);
    //Initial Timer0 to periodic mode with 1Hz
    TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 1);
    TIMER_Open(TIMER1, TIMER_PERIODIC_MODE, 1);
    //Enable Timer0 interrupt
    TIMER_EnableInt(TIMER0);
    NVIC_EnableIRQ(TMR0_IRQn);
    TIMER_EnableInt(TIMER1);
    NVIC_EnableIRQ(TMR1_IRQn);

    //Initial 7-Segment
    Open_Seven_Segment();

    //Start Timer0
    TIMER_Start(TIMER0);
        TIMER_Start(TIMER1);

    while(1)
    {
        Show_Seven_Segment(TimerCounter / 10, 1);
        CLK_SysTickDelay(200);
        Show_Seven_Segment(TimerCounter % 10, 2);
        CLK_SysTickDelay(200);
    }
}

/*** (C) COPYRIGHT 2014~2015 Nuvoton Technology Corp. ***/


