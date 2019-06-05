#include <stdint.h>
#include <stdio.h>

#include <generated/csr.h>
#include <generated/mem.h>
#include <time.h>
#include <uart.h>

#include "contiki.h"
#include "contiki-net.h"
#include "liteethmac-drv.h"
#include "net/ip/dhcpc.h"

#include "ethernet.h"

#define uip_ipaddr_to_quad(a) (a)->u8[0],(a)->u8[1],(a)->u8[2],(a)->u8[3]

static int uip_periodic_event;
static int uip_periodic_period;

static int uip_arp_event;
static int uip_arp_period;

void uip_log(char *msg)
{
#ifdef UIP_DEBUG
    puts(msg);
#endif
}

static const char hex_table[16] = "0123456789ABCDEF";

static void dbg_printhex(unsigned int x, unsigned char size) {
	unsigned char i, j;
	
	for (i = 0; i < 8; i++) {
		j = (x & 0xF0000000) >> 28;
		x = x << 4;
		
		if (i >= (8 - size)) {
			uart_write(hex_table[j]);
		}
	}
}

void ethernet_init(const unsigned char *mac_addr, const unsigned char *ip_addr) {
	unsigned int i;
	uip_ipaddr_t ipaddr;

	clock_init();
	liteethmac_init();

	uip_periodic_period = SYSTEM_CLOCK_FREQUENCY/100;
	uip_arp_period = SYSTEM_CLOCK_FREQUENCY/10;

	/* init uip */
	process_init();
	process_start(&etimer_process, NULL);
	uip_init();

	/* configure mac / ip */
	for (i=0; i<6; i++) uip_lladdr.addr[i] = mac_addr[i];
	
	uip_ipaddr(&ipaddr, ip_addr[0], ip_addr[1], ip_addr[2], ip_addr[3]);
	uip_sethostaddr(&ipaddr);

	printf("uIP init done with ip %d.%d.%d.%d\n", ip_addr[0], ip_addr[1], ip_addr[2], ip_addr[3]);
}

void ethernet_service(void) {
	int i;
	struct uip_eth_hdr *buf = (struct uip_eth_hdr *)&uip_buf[0];

	etimer_request_poll();
	process_run();

	uip_len = liteethmac_poll();
	if(uip_len > 0) {
		if(buf->type == uip_htons(UIP_ETHTYPE_IP)) {
			printf("IP");
			uip_arp_ipin();
			uip_input();
			if(uip_len > 0) {
				uip_arp_out();
				liteethmac_send();
			}
		} else if(buf->type == uip_htons(UIP_ETHTYPE_ARP)) {
			printf("ARP");
			uip_arp_arpin();
			if(uip_len > 0)
				liteethmac_send();
		} else if(buf->type == uip_htons(UIP_ETHTYPE_IPV6)) {
			printf("IPV6");
		} else {
			printf("Unknown type");
		}

		printf(" / Src:");
		for (i = 0; i < 6; i++) {
			dbg_printhex(buf->src.addr[i], 2);
		}
		printf(" Dest:");
		for (i = 0; i < 6; i++) {
			dbg_printhex(buf->dest.addr[i], 2);
		}
		printf("\n");
	} else if (elapsed(&uip_periodic_event, uip_periodic_period)) {
		for(i = 0; i < UIP_CONNS; i++) {
			uip_periodic(i);

			if(uip_len > 0) {
				uip_arp_out();
				liteethmac_send();
			}
		}
	}

	if (elapsed(&uip_arp_event, uip_arp_period)) {
		uip_arp_timer();
	}
}
