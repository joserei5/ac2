#include <detpic32.h>

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

// configure Timer T3
// f_out = frequency output for timer3
void configT3(int f_out)
{
    /* Math for Timer T3 */
    float Kdecimal;
    int K, Kindex, PR3VAL;
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
    printStr("\n\n");
}

int main(void)
{
    configT3(10);

    while(1)
    {
        while(IFS0bits.T3IF == 0);
        IFS0bits.T3IF = 0;
        putChar('.');
    }

    return 0;
}

