
C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c graphics/*.c util/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h graphics/*.h util/*.h)
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o}

CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb
LD = /usr/local/i386elfgcc/bin/i386-elf-ld

CFLAGS = -g -m32 -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs \
		 -Wall -Wextra -Werror

clean:
	rm -f *.bin *.o *.img *.tmp **/*.bin **/*.o **/*.tmp **/*.img	
		
	
run:
	make os-image.img
	"C:/Program Files (x86)/Bochs-2.6.8/bochs.exe" -f "C:/Program Files (x86)/Bochs-2.6.8/bochsrc.bxrc" -q

os-image.img: boot/bootsect.bin kernel.bin
	cat $^ > $@
	
boot/bootsect.bin:  boot/bootsect.asm boot/**/*.asm
	nasm $< -f bin -I "boot/" -o $@ 
	
kernel.bin: kernel/kernel_entry.o ${OBJ}
	${LD} -o $@ -Ttext 0x1000 $^ --oformat binary #Ttext means where this code will be put into; its same
												  #as assembler's org directive
	
kernel.elf: kernel/kernel_entry.o ${OBJ}
	${LD} -o $@ -Ttext 0x1000 $^
	
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@
		
%.o: %.asm
	nasm $< -f elf -o $@

	

	
