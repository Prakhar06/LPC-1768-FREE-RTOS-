# unknown Author


# This is an AMAZING MAKEFILE


# The name of your project (used to name the compiled .hex file)
TARGET = main



INCLUDES = \
-I. \
-I./FreeRTOS/include \
-I./FreeRTOS/portable/GCC/ARM_CM3 \
-I./Minimal/include \


# compiler options for C only
CFLAGS = -W -Wall -Os --std=gnu99 -fgnu89-inline -mcpu=cortex-m3 -mthumb
CFLAGS  += -ffunction-sections -fdata-sections $(INCLUDES)

# linker options
LDFLAGS = -Os -Wl,--gc-sections $(INCLUDES) -mcpu=cortex-m3 -mthumb -TLPC17xx.ld -Xlinker -Map=demp.map

# additional libraries to link
LIBS = -lm


# names for the compiler programs
CC = arm-none-eabi-gcc
CXX = arm-none-eabi-g++
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# automatically create lists of the sources and objects
# TODO: this does not handle Arduino libraries yet...
C_FILES := $(wildcard ./*.c) \
$(wildcard ./FreeRTOS/*.c) \
$(wildcard ./FreeRTOS/portable/GCC/ARM_CM3/*.c) \
$(wildcard ./FreeRTOS/portable/MemMang/*.c) \
$(wildcard ./Minimal/*.c) 


OBJS := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o)


# the actual makefile rules (all .o files built by GNU make's default implicit rules)

all: $(TARGET).hex $(TARGET).bin
# SOME IMPLICIT THING, INSANE
$(TARGET).elf: $(OBJS) LPC17xx.ld
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

%.hex: %.elf
	$(SIZE) $<
	$(OBJCOPY) -O ihex -R .eeprom $< $@

%.bin: %.elf
	$(OBJCOPY) $(CPFLAGS) -O binary $< $*.bin
# compiler generated dependency info
-include $(OBJS:.o=.d)

isp: $(TARGET).bin
	mdel -i /dev/sdc ::/firmware.bin
	mcopy -i /dev/sdc $(TARGET).bin ::/
clean:
	find . -type f -name '*.o' -or -name '*.d' -or -name $(TARGET).elf -or -name $(TARGET).hex -delete


