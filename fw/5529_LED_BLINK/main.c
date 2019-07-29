#include <msp430.h> 

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    P1DIR |= BIT0;

    volatile unsigned int i;

    while(1)
    {
        P1OUT ^= BIT0;
        for(i=10000; i>0; i--);
    }
}
