#include <detpic32.h>

void initAll(void)
{
    /*I/O*/
    //mask
    LATB = LATB & 0xFFF0;
    //inputs
    TRISB = TRISB | 0x0F;
    
    /*ADC*/
    // mask
    LATBbits.LATB4 = 0;
    //input
    TRISBbits.TRISB4 = 1;
    //analog
    AD1PCFGbits.PCFG4 = 0;
    AD1CHSbits.CH0SA = 4;
    //conversions
    AD1CON1bits.SSRC = 7;
    AD1CON2bits.SMPI = 16-1;
    AD1CON3bits.SAMC = 16;
    AD1CON1bits.CLRASAM = 1;
    //activate
    AD1CON1bits.ON = 1;
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
    int *reg = (int *)(&ADC1BUF0);
    int val_adc = 0;
    int i, freq;
    
    while(1)
    {
        AD1CON1bits.ASAM = 1; // start ADC
        while(IFS1bits.AD1IF == 0); //wait for interrupt
        IFS1bits.AD1IF = 0; // reset ADC flag
        
        for(i=0; i<16; i++)
        {
            val_adc += reg[i*4];
        }
        val_adc = val_adc/16;
        freq = 1 + val_adc/255;
        freq = 1000/freq;
        
        printInt10(PORTBbits.RB3);
        printInt10(PORTBbits.RB2);
        printInt10(PORTBbits.RB1);
        printInt10(PORTBbits.RB0);
        putChar('\n');
        delay(freq);
    }
    
    return 0;
}
