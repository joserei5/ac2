#include <detpic32.h>

void delay(int ms)
{
  for(; ms>0; ms--)
  {
    resetCoreTimer();
    while(readCoreTimer() < 20000);
  }
}

int main(void)
{
  TRISE = TRISE & 0xFFF0;
  LATE = LATE & 0xFFF0;

  unsigned char counter = 0;

  while(1)
  {
    delay(250);
    LATE = (LATE & 0xFFF0) | counter;
    counter++;
  }
  return 0;
}
