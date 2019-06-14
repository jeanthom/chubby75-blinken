include ../include/generated/variables.mak
include $(SOC_DIRECTORY)/software/common.mak

BUILD_DIRECTORY=$(BUILDINC_DIRECTORY)/../../
FIRMBUILD_DIRECTORY=$(BUILD_DIRECTORY)/software/firmware

OBJECTS=\
	main.o \
	irq.o \
	shell.o \
	j600io.o \
	../libnet/microudp.o \
	boot-helper-$(CPU).o

GIT_VERSION := $(shell git describe --abbrev=7 --dirty --always --tags)

CFLAGS += \
	-I$(FIRMWARE_DIRECTORY) \
	-I$(BUILD_DIRECTORY)/software/firmware

CFLAGS += \
	-Wall \
	-Werror \
	-O3 \
	-D__GITVERSION__=\"$(GIT_VERSION)\"

LDFLAGS +=

ifeq ($(COPY_TO_MAIN_RAM), 1)
CRT0 = ../libbase/crt0-$(CPU)-ctr.o
LINKER_LD = linker-ctr.ld
else
CRT0 = ../libbase/crt0-$(CPU)-xip.o
LINKER_LD = linker-xip.ld
endif

all: firmware.bin firmware.fbi

%.fbi: %.bin
ifeq ($(CPUENDIANNESS), little)
	$(PYTHON) -m litex.soc.software.mkmscimg -f --little $< -o $@
else
	$(PYTHON) -m litex.soc.software.mkmscimg -f $< -o $@
endif

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@
	chmod -x $@

firmware.elf: $(FIRMWARE_DIRECTORY)/$(LINKER_LD) $(OBJECTS)

%.elf: $(CRT0) ../libbase/libbase-nofloat.a ../libcompiler_rt/libcompiler_rt.a
	$(LD) $(LDFLAGS) -L$(FIRMWARE_DIRECTORY) -T $(FIRMWARE_DIRECTORY)/$(LINKER_LD) -N -o $@ \
		$(CRT0) \
		$(OBJECTS) \
		-L../libbase \
		-L../libcompiler_rt \
		-L../libnet \
		-lbase-nofloat -lcompiler_rt
	chmod -x $@

# pull in dependency info for *existing* .o files
-include $(OBJECTS:.o=.d)

boot-helper-$(CPU).S: $(BIOS_DIRECTORY)/boot-helper-$(CPU).S
	cp $< $@

boot-helper-$(CPU).o: boot-helper-$(CPU).S
	$(assemble)

%.o: $(FIRMWARE_DIRECTORY)/%.c
	$(compile)

%.o: $(FIRMWARE_DIRECTORY)/%.S
	$(assemble)

clean:
	$(RM) $(OBJECTS) $(OBJECTS:.o=.d) firmware.elf firmware.bin .*~ *~

.PHONY: all clean libs version_data