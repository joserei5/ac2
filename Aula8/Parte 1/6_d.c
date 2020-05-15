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

int main(void)
{
    configUart1(115200, 'N', 1);
    
    //configurar timer T5
    PR5 = 65535;
    T5CON |= 0x8000;

    // variaveis com tempo
    int toc = 0;

    while(1)
    {
        // esperar que registo fique fazio
        while(U1STAbits.TRMT == 0);

        // LED ON
        TMR5 = 0;

        puts("123456789AB");
        
        toc = TMR5;
        // LED OFF

        printStr("\n\rTempo (ns): ");
        printInt10(toc*50);
        printStr("\n\rTempo por char (ns): ");
        printInt10(toc*50/11);
        delay(500);
        putChar('\n');
    }

    return 0;
}

