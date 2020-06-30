#include <detpic32.h>

//macros
#define startADC()  AD1CON1bits.ASAM=1;

//global var
volatile int val_adc = 1;
volatile int *reg = (int *)(&ADC1BUF0);
volatile int i;
volatile int freq = 1;

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
    //interrupts
    IPC6bits.AD1IP = 5;
    IEC1bits.AD1IE = 1;
    IFS1bits.AD1IF = 0;
    //activate
    AD1CON1bits.ON = 1;
    
    /*DISPLAY*/
    //mask
    LATDbits.LATD6 = 0;
    LATDbits.LATD5 = 0;
    LATB = LATB & 0x00FF;
    //I/O
    TRISDbits.TRISD6 = 0;
    TRISDbits.TRISD5 = 0;
    TRISB = TRISB & 0x00FF;
    
    /*TIMER2*/
    // prescaler and registers
    T2CONbits.TCKPS = 1; //K=8
    PR2 = 24999;
    TMR2 = 0;
    // interrupts
    IPC2bits.T2IP = 5;
    IEC0bits.T2IE = 1;
    IFS0bits.T2IF = 0;
    // activate
    T2CONbits.ON = 1;
    
}

void send2displays(unsigned char value)
{
    const unsigned char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F,\
                                            0x66, 0x6D, 0x7D, 0x07,\
                                            0x7F, 0x6F, 0x77, 0x7C,\
                                            0x39, 0x5E, 0x79, 0x71};
    
    unsigned char dh = value >> 4;
    unsigned char dl = value & 0x0F;
    static char flag = 0;
    
    if(flag == 0)
    // DISPLAY low
    {
        LATDbits.LATD6 = 0;
        LATDbits.LATD5 = 1;
        LATB = ((LATB & 0x00FF) | display7Scodes[dl] << 8);
    }
    else
    // DISPLAY HIGH
    {
        LATDbits.LATD6 = 1;
        LATDbits.LATD5 = 0;
        LATB = ((LATB & 0x00FF) | display7Scodes[dh] << 8);
        LATBbits.LATB15 = 1; //decimal point
    }
    
    //toggle flag
    flag = !flag;
}

char toBcd(unsigned char value)
{
    return (((value/10)<<4) + (value%10));
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
    EnableInterrupts();
    startADC();
    
    int delay_ms = 1000;
    
    while(1)
    {
        freq = 1 + val_adc/255;
        delay_ms = 1000/freq;
        
        printInt10(PORTBbits.RB3);
        printInt10(PORTBbits.RB2);
        printInt10(PORTBbits.RB1);
        printInt10(PORTBbits.RB0);
        putChar('\n');
        delay(delay_ms);
    }
    
    return 0;
}

void _int_(27) isr_adc(void)
{
    for(i=0; i<16; i++)
    {
        val_adc += reg[i*4];
    }
    val_adc = val_adc/16;
    
    startADC();
    IFS1bits.AD1IF = 0;
}

void _int_(8) isr_T2(void)
{
    send2displays(toBcd(freq*10));
    IFS0bits.T2IF = 0;
}
