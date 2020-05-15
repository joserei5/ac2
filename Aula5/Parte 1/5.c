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

  if(!displayFlag) // LOW
  {
    LATDbits.LATD6 = 0;
    LATDbits.LATD5 = 1;
    LATB = (LATB & 0x00FF) | (display7Scodes[dl] << 8);
  }
  else // HIGH
  {
    LATDbits.LATD6 = 1;
    LATDbits.LATD5 = 0;
    LATB = (LATB & 0x00FF) | (display7Scodes[dh] << 8);
    LATBbits.LATB15 = 1; // decimal point only on HIGH disp
  }
  displayFlag = !displayFlag;
}

unsigned char toBcd(unsigned char value)
{
  return ((value / 10 ) << 4) + (value % 10);
}

int main(void)
{
  /* Display I/O */
  LATB = LATB & 0x00FF; // RB8-RB15 outputs on 0
  LATDbits.LATD5 = 0;
  LATDbits.LATD6 = 0;

  TRISB = (TRISB & 0x00FF); // RB8-RB15 as outputs
  TRISDbits.TRISD5 = 0;     // RD5 as output
  TRISDbits.TRISD6 = 0;     // RD6 as output

  /* A/D Converter */
  LATBbits.LATB4 = 0;     // MASK
  TRISBbits.TRISB4 = 1;   // RB4 = input
  AD1PCFGbits.PCFG4 = 0;  // RB4 = analog input
  AD1CHSbits.CH0SA = 4;   // AN4 MUX entry
  AD1CON2bits.SMPI = 4-1; // Conversions: 4

  AD1CON1bits.SSRC = 7;     // Conversion Trigger Selection Bits
  AD1CON1bits.CLRASAM = 1;  // STOPS conversion & CLEARS ASAM bit
                            // when an A/D interrupt is generated
  AD1CON3bits.SAMC = 16;    // SAMPLING TIME = 16 TAD = 16*100ns

  AD1CON1bits.ON = 1;     // ENABLE A/D Converter
  int i = 0;
  int j;
  int V, V_avg;
  while(1)
  {
    AD1CON1bits.ASAM = 1;      // START conversion
    while(IFS1bits.AD1IF == 0); // WAIT for conversion

    int *p = (int *)(&ADC1BUF0);
    V = 0;

    if(i++ % 25 == 0)
    {
      for(j=0; j<4; j++)
        V += (int)((p[j*4]*33+511)/(1024-1));

      V_avg = V/4;
    }

    printInt(V_avg, 10 | 3 << 10);
    putChar(' ');
    send2displays(toBcd(V_avg));
    delay(10);

    IFS1bits.AD1IF = 0;         // RESET conversion <=> AD1F
  }

  return 0;
}
