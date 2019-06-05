#include <stdint.h>
#include <generated/mem.h>

#include "j600io.h"

#define J600IO_U600_601 *(volatile uint16_t*)(J600IO_BASE)
//#define J600IO_U604_605 *(volatile uint8_t*)(J600IO_BASE+1)
//#define J600IO_U608 *(volatile uint8_t*)(J600IO_BASE+2)

void j600io_write_u600(uint16_t val) {
	(void)val;
	J600IO_U600_601 = val;
}
