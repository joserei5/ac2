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
        char portbits = PORTB & 0x0F;
        LATE = (LATE & 0xFFF0) | portbits;
    }
    
    return 0;
}
