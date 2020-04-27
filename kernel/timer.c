#include "timer.h"
#include "machine.h"
#include "screen.h"


void Init_Timer()
{
    Set_Timer_Frequency(1000);
}

void Set_Timer_Frequency(int hz)
{
    int divisor = 1193180 / hz;   /* Calculate our divisor */
    outb(0x43, 0x36);             /* Set our command byte 0x36 */
    outb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    outb(0x40, divisor >> 8);     /* Set high byte of divisor */
}

int tick_count = 0;
void On_Timer_Tick()
{
    tick_count ++;
    if (tick_count >= 1000) {
        tick_count -= 1000;
    }
}