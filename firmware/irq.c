#include <stdarg.h>
#include <generated/csr.h>
#include <irq.h>
#include <uart.h>

static void emerg_printf(const char *fmt, ...)
{
	char buf[512];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);

	char *p = buf;
	while(*p) {
		while(uart_txfull_read());
		uart_rxtx_write(*p++);
	}
}

void isr(void);
void isr(void) {
	unsigned int irqs;

	irqs = irq_pending() & irq_getmask();

	if(irqs & (1 << UART_INTERRUPT)) {
		uart_isr();
	} else if (irqs & (1 << EBREAK_ECALL_ILLEGAL_INTERRUPT)) {
		emerg_printf("EBREAK_ECALL_ILLEGAL_INTERRUPT :-(\n");
	} else if (irqs & (1 << BUS_ERROR_INTERRUPT)) {
		emerg_printf("BUS_ERROR_INTERRUPT :-(\n");
	}
}
