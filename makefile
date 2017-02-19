
C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h)
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o}

CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb
LD = /usr/local/i386elfgcc/bin/i386-elf-ld

CFLAGS = -g

clean:
	rm *.bin *.o *.img *.tmp **/*.bin **/*.o **/*.tmp **/*.img	
		
	
run:
	make os-image.img

os-image.img: boot/boot_sect.bin kernel.bin
	cat $^ > $@
	
boot/boot_sect.bin:  boot/boot_sect.asm boot/**/*.asm
	nasm $< -f bin -I "boot/" -o $@
	
kernel.bin: kernel/kernel_entry.o ${OBJ}
	${LD} -o $@ -Ttext 0x1000 $^ --oformat binary
	
kernel.elf: kernel/kernel_entry.o ${OBJ}
	${LD} -o $@ -Ttext 0x1000 $^
	
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@
		
%.o: %.asm
	nasm $< -f elf -o $@

	

	
