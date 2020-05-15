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
  static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F,\
                                        0x66, 0x6D, 0x7D, 0x07,\
                                        0x7F, 0x6F, 0x77, 0x7C,\
                                        0x39, 0x5E, 0x79, 0x71};
  unsigned char dh = value >> 4;
  unsigned char dl = value & 0x0F;
  static char displayFlag = 0;

  if(!displayFlag)
  {
    LATDbits.LATD6 = 0;
    LATDbits.LATD5 = 1;
    LATB = (LATB & 0x80FF) | (display7Scodes[dl] << 8);
  }
  else
  {
    LATDbits.LATD6 = 1;
    LATDbits.LATD5 = 0;
    LATB = (LATB & 0x80FF) | (display7Scodes[dh] << 8);
  }

  displayFlag = !displayFlag;
}

int main(void)
{
  unsigned char counter = 0;
  unsigned int i;

  LATB = LATB & 0x80FF; // RB8-RB14 outputs on 0
  LATDbits.LATD5 = 0;
  LATDbits.LATD6 = 0;

  TRISB = (TRISB & 0x80FF); // RB8-RB14 as outputs
  TRISDbits.TRISD5 = 0;     // RD5 as output
  TRISDbits.TRISD6 = 0;     // RD6 as output

  while(1)
  {
    i = 0;
    do
    {
      send2displays(counter);
      delay(20);
    } while(++i<10);
    counter = (counter+1)%256;
  }
  return 0;
}
