#include <detpic32.h>

//global var
volatile char pressedKey = 0;
volatile char count = 0;
volatile char actualCount = 0;

void send2displays(unsigned char value)
{
    const unsigned char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F,\
                                            0x66, 0x6D, 0x7D, 0x07,\
                                            0x7F, 0x6F, 0x77, 0x7C,\
                                            0x39, 0x5E, 0x79, 0x71};
    
    unsigned char dh = value >> 4;
    unsigned char dl = value & 0x0F;
    static char flag = 0 ;
    
    if(flag==0)
    // display LOW
    {
        LATDbits.LATD6 = 0;
        LATDbits.LATD5 = 1;
        LATB = ((LATB & 0x80FF) | (display7Scodes[dl] << 8));
    }
    else
    // display HIGH
    {
        LATDbits.LATD6 = 1;
        LATDbits.LATD5 = 0;
        LATB = ((LATB & 0x80FF) | (display7Scodes[dh] << 8));
    }
    
    //toggle flag 
    flag = !flag;
}

char toBcd(unsigned char value)
{
    return (((value/10)<<4) + (value%10));
}

void initAll(void)
{
    /*Displays*/
    // masks
    LATDbits.LATD6 = 0;
    LATDbits.LATD5 = 0;
    LATD = LATD & 0x80FF;
    // I/O
    TRISDbits.TRISD6 = 0;
    TRISDbits.TRISD5 = 0;
    TRISB = TRISB & 0x80FF;
    
    /*Timer3*/
    // prescaler and registers
    // fout = 10Hz
    T3CONbits.TCKPS = 5; // K=32
    PR3 = 62499;
    TMR3 = 0;
    //interrupts
    IPC3bits.T3IP = 5; // priority
    IEC0bits.T3IE = 1; // activate
    IFS0bits.T3IF = 0; // flag
    //activate timer
    T3CONbits.ON = 1;
    
    /*Timer4*/
    // prescaler and registers
    // fout = 50Hz
    T4CONbits.TCKPS = 3; // K=8
    PR4 = 49999;
    TMR4 = 0;
    //interrupts
    IPC4bits.T4IP = 5; // priority
    IEC0bits.T4IE = 1; // activate
    IFS0bits.T4IF = 0; // flag
    //activate timer
    T4CONbits.ON = 1;
}

int main(void)
{
    initAll();
    EnableInterrupts();
    
    char c = 0;
    
    while(1)
    {
        c = 0;
        c = inkey();
        
        switch(c)
        {
            case '0':
                // 10*1 Hz
                T3CONbits.TCKPS = 5; //K=32
                PR3 = 62499;
            break;
            
            case '1':
                // 10*2 Hz
                T3CONbits.TCKPS = 4; //K=16
                PR3 = 62499;
            break;
            
            case '2':
                // 10*3Hz
                T3CONbits.TCKPS = 4; //K=16
                PR3 = 41666;
            break;
            
            case '3':
                // 10*4 Hz
                T3CONbits.TCKPS = 3; //K=8
                PR3 = 62499;
            break;
            
            case '\n':
                pressedKey = 1;
                actualCount = count;
            break;
            
            default:
            break;
        }
    }
    
    return 0;
}

void _int_(12) isr_T3(void)
{
    printInt(count, 10 | 2 << 16);
    count = (count+1)%100;
    
    if(pressedKey == 1)
    {
        printStr(" >> ");
        printInt(actualCount, 10 | 2 << 16);
    }
        
    putChar('\r');
        
    IFS0bits.T3IF = 0; //reset interrupt flag
}

void _int_(16) isr_T4(void)
{
    send2displays(toBcd(actualCount));
    IFS0bits.T4IF = 0;
}


