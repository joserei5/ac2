#include <detpic32.h>

void delay(int ms)
{
    for(;ms>0;ms--)
    {
        resetCoreTimer();
        while(readCoreTimer()<20000);
    }
}

void configAll(void)
{
    /* Outputs */
    TRISE = TRISE & 0xFFF0; // RE[3..0]
}

int main(void)
{
    configAll();
    char c = 0;
    
    while(1)
    {
        c = inkey();
        switch(c)
        {
            case '0':
                LATE = (LATE & 0xFFF0) | 0x01;
                c = 0;
            break;
            
            case '1':
                LATE = (LATE & 0xFFF0) | 0x02;
                c = 0;
            break;
            
            case '2':
                LATE = (LATE & 0xFFF0) | 0x04;
                c = 0;
            break;
            
            case '3':
                LATE = (LATE & 0xFFF0) | 0x08;
                c = 0;
            break;
            
            default:
                if(c != 0)
                {
                    LATE = (LATE & 0xFFF0) | 0x0F;
                    delay(1000);
                    LATE = (LATE & 0xFFF0);
                }
                c = 0;
            break;
        }
    }
    
    return 0;
}
