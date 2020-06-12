#include <detpic32.h>

void delay(unsigned int ms)
{
    for(;ms>0;ms--)
    {
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

int main(void)
{
    char count = 0;
    
    while(1)
    {
        printInt(count, 10 | 2 << 16);
        putChar('\r');
        count = (count+1)%100;
        delay(100); // 10Hz
    }
    
    return 0;
}

