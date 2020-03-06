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
  unsigned char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F,\
                                    0x66, 0x6D, 0x7D, 0x07,\
                                    0x7F, 0x6F, 0x77, 0x7C,\
                                    0x39, 0x5E, 0x79, 0x71};

  unsigned char counter = 0;

  LATDbits.LATD6 = 1; // display HIGH on
  LATDbits.LATD5 = 0; // display LOW off
  LATB = LATB & 0x80FF; // RB8-RB14 outputs on 0

  TRISB = TRISB & 0x80FF; // RB8-RB14 as outputs
  TRISDbits.TRISD5 = 0;   // RD5 as output
  TRISDbits.TRISD6 = 0;   // RD6 as output

  while(1)
  {
    LATB = (LATB & 0x80FF) | (display7Scodes[counter] << 8);

    delay(500);
    counter ++;
    if(counter == 16) counter = 0;
  }
  return 0;
}
