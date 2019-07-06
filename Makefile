CFLAGS=-Wall -Werror -Os -march=i686 -ffreestanding -I. -m16
CFLAGS+=-fno-tree-loop-optimize -fomit-frame-pointer

.PHONY: all disk clean run
all: boot command

%.c.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

boot: boot.c.o disk.c.o
	$(LD) $(LDFLAGS) -static -Tboot.ld -melf_i386 -nostdlib \
	--nmagic -o $@.elf $^ && objcopy -O binary $@.elf $@.bin

command: command.c.o io.c.o
	$(LD) $(LDFLAGS) -static -Tcommand.ld -melf_i386 -nostdlib \
	--nmagic -o $@.elf $^ && objcopy -O binary $@.elf $@.bin

disk: boot command
	dd if=/dev/zero of=floppy.img bs=1024 count=1440
	dd if=boot.bin of=floppy.img bs=1 count=512 conv=notrunc
	dd if=command.bin of=floppy.img seek=512 bs=1 conv=notrunc

clean:
	rm -f floppy.img *~ *.elf *.bin *.o

run: clean disk
	qemu-system-i386 -fda floppy.img -boot a

