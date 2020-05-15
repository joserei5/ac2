#include <detpic32.h>

void delay(int ms)
{
  for(; ms>0; ms--)
  {
    resetCoreTimer();
    while(readCoreTimer() < 20000);
  }
}

void ConfigureAll()
{
  /* ADC Configuration */
  LATBbits.LATB4 = 0;     // RB4 MASK
  TRISBbits.TRISB4 = 1;   // RB4 = input
  AD1PCFGbits.PCFG4 = 0;  // RB4 = analog input
  AD1CHSbits.CH0SA = 4;   // AN4 MUX entry
  AD1CON2bits.SMPI = 1-1; // Conversions: 1
  AD1CON1bits.SSRC = 7;     // Conversion Trigger Selection Bits
  AD1CON1bits.CLRASAM = 1;  // STOPS conversion & CLEARS ASAM bit
                            // when an A/D interrupt is generated
  AD1CON3bits.SAMC = 16;    // SAMPLING TIME = 16 TAD = 16*100ns
  AD1CON1bits.ON = 1;     // ENABLE A/D Converter
}

int main(void)
{
  int tmrVal;

  ConfigureAll();

  PR5 = 65535;
  T5CON |= 0x8000;

  while(1)
  {
    TMR5 = 0; // RESET T5

    AD1CON1bits.ASAM = 1;       // START Conversion
    while(IFS1bits.AD1IF == 0); // WAIT Conversion
    tmrVal = TMR5;

    printStr("\nConv. Time=");
    printInt10(tmrVal * 50);
    printStr(" ns - ");
    printInt10(ADC1BUF0);

    IFS1bits.AD1IF = 0;  // RESET Conversion

    delay(500);
  }

  return 0;
}
