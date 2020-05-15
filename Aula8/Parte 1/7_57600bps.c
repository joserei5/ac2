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
    configUart1(57600, 'N', 1);
    
    //configurar timer T5
    PR5 = 65535;
    T5CON |= 0x8000;

    // variaveis com tempo
    int toc1 = 0;
    int toc2 = 0;
    int toc3 = 0;
    int toc4 = 0;

    // strings
    char *str1 = "12345";
    char *str2 = "123456789";
    char *str3 = "123456789A";
    char *str4 = "123456789AB";

    // esta versao com todos os registos de tempo
    // difere apenas 10ns das outras mediçoes
    while(1)
    {
        // esperar que registo fique fazio
        while(U1STAbits.TRMT == 0);
        TMR5 = 0;
        puts(str1);
        toc1 = TMR5;
        putChar(' ');

        while(U1STAbits.TRMT == 0);
        TMR5 = 0;
        puts(str2);
        toc2 = TMR5;
        putChar(' ');

        while(U1STAbits.TRMT == 0);
        TMR5 = 0;
        puts(str3);
        toc3 = TMR5;
        putChar(' ');

        while(U1STAbits.TRMT == 0);
        TMR5 = 0;
        puts(str4);
        toc4 = TMR5;
        putChar(' ');

        printStr("\n\r(1)Tempo (ns): ");
        printInt10(toc1*50);
        printStr(" |Tempo por char (ns): ");
        printInt10(toc1*50/5);

        printStr("\n\r(2)Tempo (ns): ");
        printInt10(toc2*50);
        printStr(" |Tempo por char (ns): ");
        printInt10(toc2*50/9);

        printStr("\n\r(3)Tempo (ns): ");
        printInt10(toc3*50);
        printStr(" |Tempo por char (ns): ");
        printInt10(toc3*50/10);

        printStr("\n\r(4)Tempo (ns): ");
        printInt10(toc4*50);
        printStr(" |Tempo por char (ns): ");
        printInt10(toc4*50/11);

        delay(500);
        putChar('\n');
    }

    return 0;
}

