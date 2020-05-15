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
  LATBbits.LATB4 = 0;     // MASK
  TRISBbits.TRISB4 = 1;   // RB4 = input
  AD1PCFGbits.PCFG4 = 0;  // RB4 = analog input
  AD1CHSbits.CH0SA = 4;   // AN4 MUX entry
  AD1CON2bits.SMPI = 1-1; // Conversions: 1
  AD1CON1bits.SSRC = 7;     // Conversion Trigger Selection Bits
  AD1CON1bits.CLRASAM = 1;  // STOPS conversion & CLEARS ASAM bit
                            // when an A/D interrupt is generated
  AD1CON3bits.SAMC = 16;    // SAMPLING TIME = 16 TAD = 16*100ns
  IPC6bits.AD1IP = 2;     // PRIORITY of Interrupt
  IEC1bits.AD1IE = 1;     // ENABLE A/D Interrupts
  AD1CON1bits.ON = 1;     // ENABLE A/D Converter
}

void _int_(27) isr_adc(void)
{
  printInt(ADC1BUF0, 16 | 3 << 16);
  putChar('\n');

  AD1CON1bits.ASAM = 1; // START conversion
  IFS1bits.AD1IF = 0;   // RESET conversion <=> AD1F
}

int main(void)
{
  ConfigureAll();
  EnableInterrupts();
  AD1CON1bits.ASAM = 1;      // START conversion
  while(1) { }

  return 0;
}
