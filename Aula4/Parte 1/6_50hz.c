#include <detpic32.h>

void delay(int ms)
{
  for(; ms>0; ms--)
  {
    resetCoreTimer();
    while(readCoreTimer() < 20000);
  }
}

int main(void)
{
  unsigned char segment ;
  LATDbits.LATD6 = 1; // display HIGH on
  LATDbits.LATD5 = 0; // display LOW off
  LATB = LATB & 0x80FF; // RB8-RB14 outputs on 0

  TRISB = TRISB & 0x80FF; // RB8-RB14 as outputs
  TRISDbits.TRISD5 = 0;   // RD5 as output
  TRISDbits.TRISD6 = 0;   // RD6 as output

  while(1)
  {
    LATDbits.LATD6 = !LATDbits.LATD6;
    LATDbits.LATD5 = !LATDbits.LATD5;
    segment = 1;

    int i;
    for(i=0; i<7; i++)
    {
      LATB = (LATB & 0x80FF) | (segment << 8);
      delay(20);
      segment = segment << 1;
    }

  }
  return 0;
}
