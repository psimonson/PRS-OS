CFLAGS=-Wall -Werror -Os -march=i686 -ffreestanding -fno-stack-protector
CFLAGS+=-fomit-frame-pointer -I. -m32

.PHONY: all disk clean run
all: boot

%.c.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

boot: boot.c.o io.c.o
	$(LD) $(LDFLAGS) -static -Tboot.ld -melf_i386 -nostdlib --nmagic -o $@.elf $^
	objcopy -O binary $@.elf $@.bin

disk:
	dd if=/dev/zero of=floppy.img bs=1024 count=1440
	dd if=boot.bin of=floppy.img bs=1 count=512 conv=notrunc

clean:
	rm -f floppy.img *~ *.elf *.bin *.o

run: clean boot disk
	qemu-system-i386 -fda floppy.img -boot a

