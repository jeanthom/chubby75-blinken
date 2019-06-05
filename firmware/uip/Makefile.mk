LIBUIPDIR=../../vendor/libuip
UIPDIR=../../liteeth/liteeth/software/libuip

CFLAGS += \
	-I$(UIP_DIRECTORY)/$(UIPDIR) \
	-I$(UIP_DIRECTORY)/$(LIBUIPDIR) \
	-I$(UIP_DIRECTORY)/$(LIBUIPDIR)/net \
	-Wno-char-subscripts \
	-fno-strict-aliasing -fpack-struct
