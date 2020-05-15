#include <detpic32.h>

int main(void)
{
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
  while(1)
  {
    AD1CON1bits.ASAM = 1;      // START conversion
    while(IFS1bits.AD1IF == 0); // WAIT for conversion

    int *p = (int *)(&ADC1BUF0);
    int i;
    int V = 0;

    for(i=0; i<4; i++)
      V += (int)((p[i*4]*33+511)/(1024-1));

    V = V/4;
    printInt(V, 10 | 3 << 10);
    putChar(' ');
    IFS1bits.AD1IF = 0;         // RESET conversion <=> AD1F;
  }

  return 0;
}
