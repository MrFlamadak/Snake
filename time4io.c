#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

int getsw(void)
{
    return((PORTD >> 8) & 0xf);     // shift PORTD 8 bits right, then mask the 4 LSB
}

int getbtns(void)
{
    return((PORTD >> 5) & 0x7);     // shift PORTD 5 bits right, then mask the 3 LSB
}


int getbtnone(void)
{
    return((PORTF & 0x2 ));
}