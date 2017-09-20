ARMGNU  = arm-linux-gnueabihf
COPS    = -nostdlib -O0
ARCH    = -mcpu=cortex-a9 -mfpu=vfpv3 
SOURCEFILES = main.c $(wildcard lib/*.c)
OBJECTFILES = $(SOURCEFILES:.c=.o)

gcc : main.bin

all : gcc clang

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img
	rm -f *.bc

clang: main.bin

startup.o : startup.s
	$(ARMGNU)-as $(ARCH) startup.s -o startup.o

%.o: %.c 
	$(ARMGNU)-gcc $(COPS) $(ARCH) -c -o $@ $<

main.bin : memmap.ld startup.o $(OBJECTFILES)
	$(ARMGNU)-ld $(COPS) startup.o $(OBJECTFILES) -T memmap.ld -o main.elf
	$(ARMGNU)-objdump -D main.elf > main.list
	$(ARMGNU)-objcopy main.elf -O ihex main.hex
	$(ARMGNU)-objcopy main.elf -O binary main.bin
