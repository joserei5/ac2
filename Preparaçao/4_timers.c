#include <detpic32.h>

//global variables
volatile char disabledisp = 0;
volatile char c_bcd = 0;
volatile char otherkey = 0;
volatile char count = 0;

void send2displays(unsigned char value)
{
    const unsigned char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F,\
                                            0x66, 0x6D, 0x7D, 0x07,\
                                            0x7F, 0x6F, 0x77, 0x7C,\
                                            0x39, 0x5E, 0x79, 0x71};
    unsigned char dh = value >> 4;
    unsigned char dl = value & 0x0F;
    static char toggleDisp = 0;
    
    if(toggleDisp == 0 && disabledisp == 0)
    // D. LOW
    {
        LATDbits.LATD5 = 1;
        LATDbits.LATD6 = 0;
        LATB = ((LATB & 0x80FF) | display7Scodes[dl] << 8);
    }
    else if(toggleDisp == 1 && disabledisp == 0)
    // D. HIGH
    {
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 1;
        LATB = ((LATB & 0x80FF) | display7Scodes[dh] << 8);        
    }
    else if(disabledisp == 1)
    {
        LATDbits.LATD5 = 0;
        LATDbits.LATD6 = 0;
    }
    
    toggleDisp = !toggleDisp;
}

unsigned char toBcd(unsigned char value)
{
    return ( ((value/10)<<4) + (value%10) );
}

void delay(int ms)
{
    for(;ms>0;ms--)
    {
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }
}

void configAll(void)
{
    /* Outputs */
    TRISB = TRISB & 0x80FF;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    
    /* Timer T2 */
    int fout = 100;
    int K = 4;
    T2CONbits.TCKPS = 2;
    PR2 = (PBCLK/K)/fout-1;
    TMR2 = 0;
    IPC2bits.T2IP = 5;
    IEC0bits.T2IE = 1;
    IFS0bits.T2IF = 0;
    T2CONbits.ON = 1;
    
}

int main(void)
{
    configAll();
    EnableInterrupts();
    char c = 0;
    
    while(1)
    {
        c = inkey();
        switch(c)
        {
            case '0':
                if(count == 0)
                {
                    c_bcd = 0;
                    disabledisp = 0;
                }
                    c = 0;
            break;
            
            case '1':
                if(count == 0)
                {
                    c_bcd = 1;
                    disabledisp = 0;
                }
                c = 0;
            break;
            
            case '2':
                if(count == 0)
                {
                    c_bcd = 2;
                    disabledisp = 0;
                }
                c = 0;
            break;
            
            case '3':
                if(count == 0)
                {
                    c_bcd = 3;
                    disabledisp = 0;
                }
                c = 0;
            break;
            
            default:
                if(c != 0 && count == 0)
                {
                    c_bcd = 0xFF;
                    disabledisp = 0;
                    otherkey = 1;
                }
                c = 0;
            break;
        }
    }
    
    return 0;
}

void _int_(8) isr_T2(void)
{

    send2displays(c_bcd);
    
    if(otherkey == 1)
    {
        count++;
        if(count == 100) // 100*10ms = 1s
        {
            disabledisp = 1;
            otherkey = 0;
            count = 0;
        }
    }
    IFS0bits.T2IF = 0;
}
