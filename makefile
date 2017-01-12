
C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)
OBJ = ${C_SOURCES:.c=.o}

clean:
	rm *.bin *.o *.img *.tmp **/*.bin **/*.o **/*.tmp **/*.img	

os-image.img: boot/boot_sect.bin kernel.bin
	cat $^ > $@

%.bin: %.asm
	nasm $< -f bin -I "boot/" -o $@
	
kernel.bin: kernel.tmp
	objcopy -O binary -j .text $< $@

kernel.tmp: kernel/kernel_entry.o ${OBJ}
	ld -T NUL -o $@ -Ttext 0x1000 $^

%.o: %.asm
	nasm -f win32 $< -o $@

	
%.o: %.c ${HEADERS}
	gcc -ffreestanding -c $< -o $@
	
