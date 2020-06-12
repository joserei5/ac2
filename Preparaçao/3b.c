#include <detpic32.h>

void configAll(void)
{
    /* Inputs */
    LATB = LATB & 0xFFF0; // mask
    TRISB = TRISB | 0x0F; // RB[3..0]
    
    /* Outputs */
    TRISE = TRISE & 0xFFF0; // RE[3..0]
}

int main(void)
{
    configAll();
    
    while(1)
    {
        LATEbits.LATE0 = (PORTB & 0x08) >> 3;
        LATEbits.LATE1 = (PORTB & 0x04) >> 2;
        LATEbits.LATE2 = (PORTB & 0x02) >> 1;
        LATEbits.LATE3 = PORTB & 0x01;
    }
    
    return 0;
}
