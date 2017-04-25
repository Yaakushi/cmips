//
// chronometer on the LCD display
// 
// external counter interrupts 4 times per second
// displays shows a tumbling wheel;  (hex) counter increments every second
//

#include "cMIPS.h"

extern int _counter_val;

#define FALSE (0==1)
#define TRUE  !FALSE

#define QUARTER 12500000

void main(void) {
  int i, j, k;
  volatile int old_val;
  int sec, min, hour;

  LCDinit();

  LCDtopLine();

#if 1
  LCDput('c');
  LCDput('M');
  LCDput('I');
  LCDput('P');
  LCDput('S');
  LCDput(' ');
  LCDput('t');
  LCDput('i');
  LCDput('m');
  LCDput('e');
  LCDput(' ');
  LCDput('[');
  LCDput('h');
  LCDput('e');
  LCDput('x');
  LCDput(']');
#else
  LCDprint( print_sp() ); // for debugging only
#endif

  LCDbotLine();

  _counter_val = 0;   // variable to accumulate number of interrupts

  old_val = _counter_val;

  // enableInterr();

  startCounter(QUARTER,TRUE); // counter will interrupt after 1/4 second

  sec = min = hour = 0;

  i = 0;

  if (SWget() != 0) {
    LCDprint( print_status() ); // for debugging only
    delay_ms(2000);
    LCDprint( print_cause() ); // for debugging only
    delay_ms(2000);
  }

  while (TRUE) {

    while (old_val == _counter_val) { // busy wait for interrupt
      delay_us(1);   // that's really stupid but we have nothing else to do
    }

    old_val = _counter_val;

    switch(i) {
    case 0:
      j = 0x09; break;
    case 1:
      j = 0x0b; break;
    case 2:
      j = 0x0c; break;
    default:
      j = 0xa;
      i = -1;
      sec += 1;
      break;
    };
    i += 1;
    DSP7SEGput(0, 0, 0, 0, (i<<2)); // blink red led
 
    LCDgotoxy(1, 2);
    LCDput(j);

    if (sec == 60) {
      min += 1;
      sec = 0;
    }

    if (min == 60) {
      hour += 1;
      min = 0;
    }

    LCDgotoxy(4, 2);

#define conv(a) ((a<10)?((a)+0x30):((a)+('a'-10)))

    k = (hour>>4) & 0x0f;
    LCDput( conv(k) );
    k = hour & 0x0f;
    LCDput( conv(k) );

    LCDput(':');

    k = (min>>4) & 0x0f;
    LCDput( conv(k) );
    k = min & 0x0f;
    LCDput( conv(k) );

    LCDput(':');

    k = (sec>>4) & 0x0f;
    LCDput( conv(k) );
    k = sec & 0x0f;
    LCDput( conv(k) );

  }

}


