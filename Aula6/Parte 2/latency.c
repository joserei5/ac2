/* NOTA:
 * Isto é experimental, ou seja,
 * pode sofrer alteraçoes significativas */

#include <detpic32.h>

// Global Vars
volatile int ON_TIME;
volatile int CONV_TIME;
volatile int CONV_VAL;
volatile int LATENCY;

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
  IPC6bits.AD1IP = 2;     // PRIORITY of Interrupt
  IEC1bits.AD1IE = 1;     // ENABLE A/D Interrupts
  AD1CON1bits.ON = 1;     // ENABLE A/D Converter
}

void _int_(27) isr_adc(void)
{
    // LED = OFF (0)
  ON_TIME = TMR5;

  CONV_VAL = ADC1BUF0; // read value from ADC registers

  // printing 2 latency values
  // 1. with conversion time measured in this source file
  // (more like an attempt)
  // 2. with conversion time measured in previous source 
  // file, being conv_time = 3500ns
  LATENCY = ON_TIME*50 - CONV_TIME*50;
  printInt10(LATENCY);
  printStr("ns of LATENCY| conv_time measured here\n");
  printInt10(ON_TIME*50 - 3500);
  printStr("ns of LATENCY| conv_time = 3500ns\n\n");

  delay(500);
    // LED = ON (1)
  TMR5 = 0;             // RESET TIMER5 VALUE (ON TIME)
  AD1CON1bits.ASAM = 1; // START Conversion
  IFS1bits.AD1IF = 0;   // RESET Conversion
  TMR4 = 0;             // RESET TIMER4 VALUE (CONVERSION TIME)
}

int main(void)
{
    PR5 = 65535;
    T5CON |= 0x8000;

    PR4 = 65535;
    T4CON |= 0x8000;

    // INIT VARIABLES
    ON_TIME = 0;
    CONV_TIME = 0;

  ConfigureAll();
  EnableInterrupts();   // ENABLE Interrupts
  AD1CON1bits.ASAM = 1; // START Conversion

  while(1)
  {
      // Conversion time
      CONV_TIME = TMR4; // updates until interrupt service is called
  }


  return 0;
}
