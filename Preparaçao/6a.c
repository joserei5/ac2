#include <detpic32.h>

//global var
volatile char pressedKey = 0;
volatile char count = 0;
volatile char actualCount = 0;

void initAll(void)
{
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
