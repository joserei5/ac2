#include <detpic32.h>

void delay(int ms)
{
  for(; ms>0; ms--)
  {
    resetCoreTimer();
    while(readCoreTimer() < 20000);
  }
}

void send2displays(unsigned char value)
{
  unsigned char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F,\
                                    0x66, 0x6D, 0x7D, 0x07,\
                                    0x7F, 0x6F, 0x77, 0x7C,\
                                    0x39, 0x5E, 0x79, 0x71};
  unsigned char dh = value >> 4;
  unsigned char dl = value & 0x0F;

    LATDbits.LATD6 = 0;
    LATDbits.LATD5 = 1;
    LATB = (LATB & 0x80FF) | (display7Scodes[dl] << 8);

    LATDbits.LATD6 = 1;
    LATDbits.LATD5 = 0;
    LATB = (LATB & 0x80FF) | (display7Scodes[dh] << 8);
}

int main(void)
{
  return 0;
}
