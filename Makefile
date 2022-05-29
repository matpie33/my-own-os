

all:  bootloader kernel bootdisk

.PHONY: bootdisk bootloader os

clean:
	rm boot.o
	rm kernel.o
	rm myos.bin
	rm myos.iso

bootloader:
	i686-elf-as boot.s -o boot.o
	
kernel:
	i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
	i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc



bootdisk: bootloader os	
	mkdir -p isodir/boot/grub
	cp myos.bin isodir/boot/myos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir
	
qemu:
	qemu-system-i386 -cdrom myos.iso -gdb tcp::26000 -S
	
	
	

