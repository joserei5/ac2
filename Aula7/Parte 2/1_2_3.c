#include <detpic32.h>

volatile static int PR3VAL;

// alternative to ceil() function,
// since we can't import libraries.
// ceil() -> rounds number to nearest integer
// e.g.: ceil(0.3) = 1; ceil(0.9) = 1
int alt_ceil(float x)
{
    int result = ((int)x);
    float decimal = x - ((int)x);
    if(decimal > 0)
    {
        result = result + 1;
    }
    return result;
}

// returns corrected K value (from registers)
// timer = 0: timerA (T1) 
// timer = 1: timerB (T2, T3, T4, T5)
int getK(int K, int timer)
{
    // register values
    const int A[4] = {1,8,64,256};
    const int B[8] = {1,2,4,8,16,32,64,256};

    //code variables
    int range;
    int shortest = 300; // making sure is far way (to initalize)
    int result = 0;

    if(timer == 0)
    {
        int i;
        for(i=0; i<4; i++)
        {
            range = A[i] - K;
            if((range < shortest) && (range >= 0))
            {
                shortest = range;
                result = A[i];
            }
        }
    }
    else if(timer == 1)
    {
        int i;
        for(i=0; i<8; i++)
        {
            range = B[i] - K;
             if((range < shortest) && (range >= 0))
            {
                shortest = range;
                result = B[i];
            }
        }

    }

    return result;
}

// returns index of K of
// the register he's in
// timer = 0: timerA (T1)
// timer = 1: timerB (T2, T3, T4, T5)
int indexK(int K, int timer)
{
    const int A[4] = {1,8,64,256};
    const int B[8] = {1,2,4,8,16,32,64,256};
    int result = 1;

    if(timer == 0)
    {
        int i = 0;
        while((A[i] != K) && (i<4))
        {
            ++i;
        }
        result = i;
    }
    else if(timer == 1)
    {
        int i = 0;
        while((B[i] != K) && (i<8))
        {
            ++i;
        }
        result = i;

    }
    
    return result;
}

// returns OCxRS value
// Duty Cycle must be: [0, 1]
int setPWM(float dcycle, int PRx)
{
    return (PRx+1)*dcycle;
}

// configure Timer T3
// f_out = frequency output for timer3
void configT3(int f_out)
{
    /* Math for Timer T3 */
    float Kdecimal;
    int K, Kindex;
    Kdecimal = (float)PBCLK/(f_out*65536);
    K = alt_ceil(Kdecimal);
    K = getK(K, 1);
    Kindex = indexK(K,1);
    PR3VAL = ((PBCLK/K)/f_out)-1;
    
    /* Config Timer T3 */
    T3CONbits.TCKPS = Kindex;   //prescaler
    PR3 = PR3VAL;               //PRx register 
    TMR3 = 0;                   //reset T3 count register
    T3CONbits.TON = 1;          //enable T3
    
    /* Prints */
    printStr("\nTIMER3: K = ");printInt10(K);
    printStr("| index  = ");printInt10(Kindex);
    printStr("| PR3 = ");printInt10(PR3VAL);
}

// configure Timer T3 PWM
// duty cycle must come in decimal format [0, 1]
void configT3_PWM(float dcycle)
{
    /* Math for Timer T3 PWM */
    int OC1VAL = setPWM(dcycle,PR3VAL);

    /* Config Timer T3 PWM */
    OC1CONbits.OCM = 6;             // PWM Mode on OC1
    OC1CONbits.OCTSEL = 1;          // Timer T3
    OC1RS = OC1VAL;                 // Ton constant
    OC1CONbits.ON = 1;              // ENABLE OC1 module

    /* Prints */
    printStr("\nOC1RS = ");printInt10(OC1VAL);
}

void delay(int ms)
{
  for(; ms>0; ms--)
  {
    resetCoreTimer();
    while(readCoreTimer() < 20000);
  }
}

void printTime(int period, int ton, int pwm)
{
   printStr("PWM period: ");
   printInt( (period * 2)/10, 10 | 5 << 16);
   printStr(" - Ton: ");
   printInt( (ton    * 2)/10, 10 | 5 << 16);
   printStr(" [us] - PWM = ");
   printInt10(pwm);
   printStr(" \%  \r");
}

int main(void)
{
    // init Timer T3
    configT3(100);
    
    // duty dycle percentage
    int duty_cycle = 25;
    configT3_PWM((float)duty_cycle/100);
    
    // init counter Timer T5
    PR5               = 65535;
    T5CONbits.TCKPS   = 2;     	// fout_presc = 5 MHz (0.2us*65535=13107us)
    T5CONbits.TON     = 1;		// 1/(100Hz) = 100000us
    int tmrVal = 0, period = 0, ton = 0, counter = 0;
    char signal = 0;

    // init input key
    char ch = 0; 
    
    while(1)
    {
        ch = inkey();
        
        if(ch == '-')
        {
            if(duty_cycle > 1)
            {
                duty_cycle--;
                configT3_PWM((float)duty_cycle/100);
            }
        }

        if(ch == '+')
        {
            if(duty_cycle < 99)
            {
                duty_cycle++;
                configT3_PWM((float)duty_cycle/100);
            }
        }


        tmrVal = TMR5;
        if(signal == 0)
        {
            if(PORTDbits.RD0 == 1)
            {
                period = tmrVal;
                TMR5 = 0;
                signal = 1;
                counter++;
            }
        }
        else
        {
            if(PORTDbits.RD0 == 0)
            {
                ton = tmrVal;
                signal = 0;
            }
        }

        if(counter >= 20)
        {
            printTime(period, ton, duty_cycle);
            counter = 0;
        }
    }

    return 0;
}

