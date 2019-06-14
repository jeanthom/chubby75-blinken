#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <generated/csr.h>
#include <irq.h>
#include <uart.h>
#include <time.h>
#include <net/microudp.h>

#include "shell.h"
#include "j600io.h"

void my_udp_callback(unsigned int src_ip, unsigned short src_port, unsigned short dst_port, void *data, unsigned int length);
void my_udp_callback(unsigned int src_ip, unsigned short src_port, unsigned short dst_port, void *data, unsigned int length) {
	uint8_t *buf;

	printf("Recevied %u bytes from %u:%d\n", length, src_ip, src_port);

	buf = (uint8_t*)data;
	printf("buf[0] = %d\n",buf[0]);

	if (buf[0] == '1') {
		J600IO_U600 = 0xFF;
	} else if (buf[0] == '0') {
		J600IO_U600 = 0x00;
	}
}

int main(void) {
	const unsigned char mac[] = { 0x83, 0x3F, 0x27, 0x2D, 0x53, 0x7F };
	const unsigned char ip_addr[] = { 172,19,1,31 };

	irq_setmask(0);
	irq_setie(1);
	uart_init();
	time_init();

	puts("Chubby75-blinken built "__DATE__" "__TIME__);
	puts("Git revision: "__GITVERSION__);
	printf("CPU type: %s\n", config_cpu_type_read());
	
	eth_init();
	microudp_start(mac, IPTOINT(ip_addr[0], ip_addr[1], ip_addr[2], ip_addr[3]));
	//shell_init();

	microudp_set_callback(my_udp_callback);

	//telnet_init();
	
	while (1) {
		//shell_service();
		//ethernet_service();
		microudp_service();
	}

	return 0;
}
