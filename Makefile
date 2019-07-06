CFLAGS=-Wall -Werror -Os -march=i686 -ffreestanding -I. -m16
CFLAGS+=-fno-tree-loop-optimize -fomit-frame-pointer

SRCDIR=$(shell pwd)

VERSION=0.1
BASENAM=$(shell basename $(SRCDIR))
TARNAME=$(BASENAM)-$(VERSION).tgz

.PHONY: all disk clean run distclean dist
all: boot command

%.c.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

boot: boot.c.o disk.c.o
	$(LD) $(LDFLAGS) -static -Tboot.ld -melf_i386 -nostdlib \
	--nmagic -o $@.elf $^ && objcopy -O binary $@.elf $@.bin

command: command.c.o io.c.o time.c.o
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

distclean: clean

dist:
	@echo Compressing $(TARNAME)...
	@cd .. && tar --exclude=.git -cf - ./$(BASENAM) | \
	gzip -9 > ./$(TARNAME) && echo Compression done! || \
	echo Compression failed.
