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
    char c = 0;
    char count = 0;
    char freq = 10;
    
    while(1)
    {
        c = 0;
        c = inkey();
        
        printInt(count, 10 | 2 << 16);
        count = (count+1)%100;
        delay(1000/freq); // ms
        
        switch(c)
        {
            case '0':
                freq = 10;
            break;
            
            case '1':
                freq = 10*2;
            break;
            
            case '2':
                freq = 10*3;
            break;
            
            case '3':
                freq = 10*4;
            break;
            
            case '\n':
                printStr(" >> ");
                printInt(count, 10 | 2 << 16);
            break;
            
            default:
            break;
        }
        
        putChar('\r');
        
    }
    
    return 0;
}

