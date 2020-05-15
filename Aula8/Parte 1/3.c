#include <detpic32.h>

void configUart1(int baudrate, char parity, int DATAbits, int STOPbits)
{
    // gerador baudrate
    U1BRG = (PBCLK+16/2*baudrate)/(16*baudrate)-1;
    U1MODEbits.BRGH = 0;

    // parametros trauma
   if(DATAbits == 8)
   { 
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

            default:
                U1MODEbits.PDSEL = 0;
                break;

        }
   }
   else
   {
        U1MODEbits.PDSEL = 3;
   }

   // stop bits
   if(STOPbits > 2) STOPbits = 2;
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

int main(void)
{
    configUart1(115200,'N',8,1);

    while(1)
    {
        puts("String de teste\n");
        delay(1000);
    }

    return 0;
}

