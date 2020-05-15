#include <detpic32.h>

void configUart1(int baudrate, char parity, int STOPbits)
{
    // gerador baudrate
    if((baudrate<600) || (baudrate >115200))
    {
         baudrate = 115200;
    }
    U1BRG = (PBCLK+16/2*baudrate)/(16*baudrate)-1;
    U1MODEbits.BRGH = 0;

    // parametros trauma
    switch(parity)
    {
        case 'n':
        case 'N':
            U1MODEbits.PDSEL = 0;
            break;

        case 'e':
        case 'E':
            U1MODEbits.PDSEL = 1;
            break;

        case 'o':
        case 'O':
            U1MODEbits.PDSEL = 2;
            break;

        default: // caso paridade não seja 'N', 'E' ou 'O'
            U1MODEbits.PDSEL = 0;
            break;

    }

   // stop bits
   if((STOPbits > 2)||(STOPbits < 1))
   {
       STOPbits = 1;
   }
   U1MODEbits.STSEL = STOPbits-1;

   // ativar modulos transmissao
   U1STAbits.UTXEN = 1;
   U1STAbits.URXEN = 1;

   // ativar UART
   U1MODEbits.ON = 1;
}

void delay(int ms)
{
    for(; ms>0; ms--)
    {
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

void putc(char byte2send)
{
    if(U1STAbits.OERR == 1)
    {
        U1STAbits.OERR = 0;
    }

    while(U1STAbits.UTXBF == 1);
    U1TXREG = byte2send;
}

void puts(char *str)
{
    while(*str != '\0')
    {
        putc(*str);
        str++;
    }

}

char getc(void)
{
    int result = 0;
    if(U1STAbits.OERR == 1)
    {
        U1STAbits.OERR = 0;
    }

    while(U1STAbits.URXDA == 0);

    if((U1STAbits.FERR == 1) || (U1STAbits.PERR == 1))
    {
        int dummy = U1RXREG;
    }
    else
    {
        result = U1RXREG;
    }

    return result;
}

int main(void)
{
    configUart1(115200, 'N', 1);
    
    while(1)
    {
        putc(getc());
    }

    return 0;
}

