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
int Putc(char c);

#define UART_SPEED	3

// Shared variables with asm
extern UARTdriver Ud;
extern int tx_has_started;

int main() {
	volatile Tserial *uart = (void *) IO_UART_ADDR;
	Tcontrol ctl;

	Ud.nrx = 0;
	Ud.ntx = 0;
	tx_has_started = 0;
	
	uart->interr.i = UART_INT_progRX;
	
	ctl.rts = 0;
	ctl.speed = UART_SPEED;
	ioctl(ctl);
	ctl.rts = 1;
	ioctl(ctl);

	Putc('o');
	Putc('l');
	Putc('a');
	Putc('\n');
	
	while(1) {
		int x = proberx();
		if(x > 0) {
			char c = Getc();
			if(c == EOT) break;
			to_stdout(c);
		}
	}
	to_stdout('e');
	to_stdout('n');
	to_stdout('d');
	to_stdout('\n');
	return 0;
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
	//volatile Tserial *uart = (void *) IO_UART_ADDR;
	//unsigned int interr = uart->interr.i;
	disableInterr();
	
	char c;
	Ud.nrx -= 1;
	c = Ud.rx_q[Ud.rx_hd];
	Ud.rx_hd += 1; // rx_hd++
	Ud.rx_hd &= Q_SZ-1; // % Q_SZ
	
	enableInterr();
	//print(uart->interr.i);
	return c;
}

int Putc(char c) {
	volatile Tserial *uart = (void *) IO_UART_ADDR;
	disableInterr();
	if(tx_has_started != 0) {
		// TX já acontecendo. Apenas coloca na fila.
		print(uart->interr.i);
		if(probetx() >= Q_SZ) {
			// Sem espaco na queue. Retorna.
			enableInterr();
			return 0;
		}
		Ud.tx_q[Ud.tx_tl++] = c;
		Ud.tx_tl %= Q_SZ;
		Ud.ntx++;
		enableInterr();
		return 1;
	} else {
		// UART não transmitindo.
		// Faz o xunxo e joga direto no TXreg.
		uart->interr.i = uart->interr.i | UART_INT_progTX;
		uart->data = (int) c;
		print(uart->interr.i);
		//print(0xABCDDCBA);
		tx_has_started = 1;
		enableInterr();
		return 1;
	}
}
