#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <generated/csr.h>
#include <irq.h>
#include <uart.h>
#include <time.h>

#include "shell.h"
#include "ethernet.h"
#include "telnet.h"

int main(void) {
	int i = 0;
	const unsigned char mac[] = { 0x83, 0x3F, 0x27, 0x2D, 0x53, 0x7F };
	const unsigned char ip_addr[] = { 172,19,1,31 };

	irq_setmask(0);
	irq_setie(1);
	uart_init();
	time_init();

	puts("Chubby75-blinken built "__DATE__" "__TIME__);
	puts("Git revision: "__GITVERSION__);
	printf("CPU type: %s\n", config_cpu_type_read());
	
	ethernet_init(mac,ip_addr);
	shell_init();

	telnet_init();
	
	while (1) {
		//shell_service();
		ethernet_service();
		//uart_sync();

		if (i == 100000) {
			printf("heartbeet\n");
			i = 0;
		}
		i++;
	}

	return 0;
}
