#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <console.h>

#include "shell.h"
#include "j600io.h"

static char *readstr(void) {
	char c[2];
	static char s[64];
	static int ptr = 0;

	if(readchar_nonblock()) {
		c[0] = readchar();
		c[1] = 0;
		switch(c[0]) {
			case 0x7f:
			case 0x08:
				if(ptr > 0) {
					ptr--;
					putsnonl("\x08 \x08");
				}
				break;
			case 0x07:
				break;
			case '\r':
			case '\n':
				s[ptr] = 0x00;
				putsnonl("\n");
				ptr = 0;
				return s;
			default:
				if(ptr >= (sizeof(s) - 1))
					break;
				putsnonl(c);
				s[ptr] = c[0];
				ptr++;
				break;
		}
	}

	return NULL;
}

static char *get_token(char **str) {
	char *c, *d;

	c = (char *)strchr(*str, ' ');
	if(c == NULL) {
		d = *str;
		*str = *str+strlen(*str);
		return d;
	}
	*c = 0;
	d = *str;
	*str = c+1;
	return d;
}

static void reboot(void) {
	//asm("call r0");
	printf("Unavailable\n");
}

static void help(void) {
	puts("Available commands:");
	puts("help        - this command");
	puts("reboot      - reboot CPU");
}

static void prompt(void) {
	printf("RUNTIME>");
}

void shell_init(void) {
	prompt();
}

void shell_service(void) {
	char *str;
	char *token;

	str = readstr();
	if(str == NULL) return;
	token = get_token(&str);
	if(strcmp(token, "help") == 0) {
		help();
	} else if(strcmp(token, "reboot") == 0) {
		reboot();
	} else if (strcmp(token, "on") == 0) {
		j600io_write_u600(0xFFFF);
	} else if (strcmp(token, "off") == 0) {
		j600io_write_u600(0);
	}

	prompt();
}
