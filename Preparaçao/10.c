/***********************
 * Executar programa com
 * pterm 1200,N,8,1
 * *********************/

#include <detpic32.h>

//macros
#define startADC()  AD1CON1bits.ASAM = 1
#define ei_u1rx()   IEC0bits.U1RXIE = 1
#define di_u1rx()   IEC0bits.U1RXIE = 0

//constants
#define BUF_SIZE 64
#define INDEX_MASK (BUF_SIZE-1)

//structures
typedef struct
{
    unsigned char data[BUF_SIZE];
    unsigned int head;
    unsigned int tail;
    unsigned int count;
} circularBuffer;

//global var
volatile int val_adc = 1;
volatile int *reg = (int *)(&ADC1BUF0);
volatile int i;
volatile int delay_ms = 1000;
volatile int freq = 1;
volatile char readc = 0;
volatile circularBuffer rxb;

void bf_flushRx()
{
    rxb.head=0;
    rxb.tail=0;
    rxb.count=0;
}

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
    
    /*UART*/
    //standard speed mode (16x baud rate clock) 
    int baudrate = 1200;
    U1BRG = ((PBCLK - 8*baudrate)/(16*baudrate))-1;
    //default
    U1MODEbits.BRGH = 0;
    U1MODEbits.PDSEL = 0;
    U1MODEbits.STSEL = 0;
    //tx and rx modules
    U1STAbits.UTXEN = 1;
    U1STAbits.URXEN = 1;
    //interrupts
    U1STAbits.URXISEL = 0;
    IEC0bits.U1EIE = 1; //uart error interrupt enable
    IPC6bits.U1IP = 6;
    IFS0bits.U1RXIF = 0;
    ei_u1rx();
    //activate
    U1MODEbits.ON = 1;
    
    /* Circular Buffer */
    bf_flushRx();
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

void putc(char byte2send)
{
    while(U1STAbits.UTXBF == 1); // wait untill tx buffer gets one position empty
    U1TXREG = byte2send; 
}

void putstr(char *str)
{
    while(*str != '\0')
    {
        putc(*str);
        str++;
    }
}

char getc(void)
{
    if(U1STAbits.OERR == 1)
    {
        return 0;
    }
    return U1RXREG;
}

void bf_update(char byte2send)
{
    // roda a atualizar os mais antigos
    rxb.data[rxb.tail] = byte2send;
    rxb.tail = (rxb.tail+1)&INDEX_MASK;
    
    di_u1rx();
    if(rxb.count<BUF_SIZE)
    {
        rxb.count++;
    }
    ei_u1rx();
}

int main(void)
{
    initAll();
    EnableInterrupts();
    startADC();
    unsigned char sequence[4]= "";
    int i;
    
    while(1)
    {
        if(readc == 'E' || readc == 'e')
        {
            putstr("\n----");
            for(i=0;i<rxb.count;i++)
            {
                if(i%4==0)
                {
                    putc('\n');
                }
                putc(rxb.data[i]);
            }
            putstr("\n----");
            readc=0;
        }
        
        sequence[0] = PORTBbits.RB3 + '0';
        sequence[1] = PORTBbits.RB2 + '0';
        sequence[2] = PORTBbits.RB1 + '0';
        sequence[3] = PORTBbits.RB0 + '0';
        
        bf_update(sequence[0]);
        bf_update(sequence[1]);
        bf_update(sequence[2]);
        bf_update(sequence[3]);
        
        putc('\n');
        putstr(sequence);
        
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
    
    freq = 1 + val_adc/255;
    delay_ms = 1000/freq;
    
    startADC();
    IFS1bits.AD1IF = 0;
}

void _int_(8) isr_T2(void)
{
    send2displays(toBcd(freq*10));
    IFS0bits.T2IF = 0;
}

void _int_(24) isr_uart1(void)
{
    if(IFS0bits.U1EIF == 1) //uart has an error
    {
        if(U1STAbits.OERR == 1)
        {
            U1STAbits.OERR = 0;
        }
        else
        {
            int dummy = U1RXREG;
        }
        IFS0bits.U1EIF = 0; //clear uart error interrupt flag
    }
    
    if(IFS0bits.U1RXIF == 1) //uart rx interrupt
    {
        readc = getc();
        IFS0bits.U1RXIF = 0; //reset uart rx flag
    }
}
