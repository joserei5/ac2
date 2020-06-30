#include <detpic32.h>

void initAll(void)
{
    /*I/O*/
    //mask
    LATB = LATB & 0xFFF0;
    //inputs
    TRISB = TRISB | 0x0F;
}

void delay(unsigned int ms)
{
    for(;ms>0;ms--)
    {
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }
}

int main(void)
{
    initAll();
    
    while(1)
    {
        printInt10(PORTBbits.RB3);
        printInt10(PORTBbits.RB2);
        printInt10(PORTBbits.RB1);
        printInt10(PORTBbits.RB0);
        putChar('\n');
        delay(1000);
    }
    
    return 0;
}
