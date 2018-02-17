#preferences for cross development

TOOLCHAIN=arm-elf-
CC=$(TOOLCHAIN)gcc
AS=$(TOOLCHAIN)as
LD=$(TOOLCHAIN)ld
OBJCOPY=$(TOOLCHAIN)objcopy

#flags

CFLAGS = -mcpu=arm7tdmi -march=armv4t -g -nostartfiles -nostdlib
ASFLAGS = 
#ASFLAGS = --D_ASSEMBLY_

BASENAME = main

ELFFILE = $(BASENAME).elf
BINARY = $(BASENAME).bin
SREC = $(BASENAME).srec
#LSCRIPT = lpc2103.ld
LSCRIPT = lpc2103_flash.cmd


#target
TARGETS = $(ELFFILE) $(BINARY) $(SREC)

OBJS = crt.o main.o

all:$(TARGETS)


$(ELFFILE): $(OBJS) Makefile
	$(LD) -o $(ELFFILE) $(OBJS) -T $(LSCRIPT) \
	-Map $(BASENAME).map


$(BINARY): $(ELFFILE)
	$(OBJCOPY) -O binary -S $(ELFFILE) $(BINARY)


$(SREC): $(ELFFILE)
	$(OBJCOPY) -O srec -S $(ELFFILE) $(SREC)

clean:
	rm -f *.o *.map $(TARGETS)

#arm-elf-gcc -c main.c -o main.o   
#sh-2.05b$ ls
#lpc2103.h  main.c  main.elf  main.o
#sh-2.05b$ arm-elf-ld -o main.elf main.o -Map main.map

