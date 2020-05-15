/* NOTA:
 * Isto é experimental, ou seja,
 * pode sofrer alteraçoes significativas */

#include <detpic32.h>

// Global Vars
volatile int EPILOG;
volatile int PROLOG;
volatile int CONV_VAL;
volatile char flag;

void delay(int ms)
{
    for(; ms>0; ms--)
    {
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

void ConfigureAll()
{
    /* ADC Configuration */
    LATBbits.LATB4 = 0;     // RB4 MASK
    TRISBbits.TRISB4 = 1;   // RB4 = input
    AD1PCFGbits.PCFG4 = 0;  // RB4 = analog input
    AD1CHSbits.CH0SA = 4;   // AN4 MUX entry
    AD1CON2bits.SMPI = 1-1; // Conversions: 1
    AD1CON1bits.SSRC = 7;     // Conversion Trigger Selection Bits
    AD1CON1bits.CLRASAM = 1;  // STOPS conversion & CLEARS ASAM bit
                            // when an A/D interrupt is generated
    AD1CON3bits.SAMC = 16;    // SAMPLING TIME = 16 TAD = 16*100ns
    IPC6bits.AD1IP = 2;     // PRIORITY of Interrupt
    IEC1bits.AD1IE = 1;     // ENABLE A/D Interrupts
    AD1CON1bits.ON = 1;     // ENABLE A/D Converter
}

void _int_(27) isr_adc(void)
{
    delay(500); // some delay to make the output readable
    printStr("\nPROLOG/LATENCY(ns): ");
    printInt10(PROLOG);
    printStr(" | EPILOG(ns): ");
    printInt10(EPILOG);
    printStr("| Global Overhead(ns): ");
    printInt10(EPILOG + PROLOG);

    CONV_VAL = ADC1BUF0; // READ value from ADC register

    AD1CON1bits.ASAM = 1; // START Converion
    IFS1bits.AD1IF = 0;   // RESET Interrupt flag

    flag = 1;
    TMR5=0;   // RESET Timer T5
}

int main(void)
{
    // INIT TIMER T5
    PR5 = 65535;
    T5CON |= 0x8000;

   // INIT VARIABLES
    PROLOG = 1150; // <=> latency
    EPILOG = 0; 

    ConfigureAll();
    EnableInterrupts();   // ENABLE Interrupts
    AD1CON1bits.ASAM = 1; // START Conversion

    while(1)
    {
        // Since the interrupt can start in the middle of the block of code
        // conditions/flags were applied
        if(flag==1)
        {
            EPILOG = TMR5*50;
        }
        flag = 0;
    }

    return 0;
}
