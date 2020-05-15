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

