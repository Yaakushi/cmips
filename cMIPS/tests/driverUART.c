#include "cMIPS.h"
#include "uart_defs.h"

// From handlers.s
void enableInterr();
void disableInterr();

// On this compiling unit
int proberx();
int probetx();
Tstatus iostat();
void ioctl(Tcontrol ctl);
char Getc();
int Putc();

#define UART_SPEED	3

// Shared variables with asm
extern UARTdriver Ud;
extern int tx_has_started;

void main() {
	volatile Tserial *uart = (void *) IO_UART_ADDR;
	Tcontrol ctl;
	Tinterr interr;

	Ud.nrx = Ud.ntx = 0;
	
	ctl.rts = 0;
	ctl.speed = UART_SPEED;
	ioctl(ctl);
	ctl.rts = 1;
	ioctl(ctl);

	interr.i = UART_INT_setRX;
	uart->interr = interr;

	while(1) {
		if(proberx() > 0) {
			char c = Getc();
			to_stdout('[');
			to_stdout(c);
			to_stdout(']');
			to_stdout('\n');
		}
	}	
}

int proberx() {
	return Ud.nrx;
}

int probetx() {
	return Ud.ntx;
}

Tstatus iostat() {
	Tserial *uart = (void *) IO_UART_ADDR;
	return uart->stat;
}

void ioctl(Tcontrol ctl) {
	volatile Tserial *uart = (void *) IO_UART_ADDR;
	uart->ctl = ctl;
}


char Getc() {
	volatile Tserial *uart = (void *) IO_UART_ADDR;
	Tinterr interr = uart->interr;
	disableInterr();
	
	char c;
	Ud.nrx -= 1;
	c = Ud.rx_q[Ud.rx_hd];
	Ud.rx_hd += 1; // rx_hd++
	Ud.rx_hd &= Q_SZ-1; // % Q_SZ
	
	enableInterr();
	interr.i = ((interr.s.intTX == 1) ? UART_INT_setTX : UART_INT_clrTX) +
		((interr.s.intRX == 1) ? UART_INT_setRX : UART_INT_clrRX);
	uart->interr = interr;
	return c;
}

int Putc() {

}
