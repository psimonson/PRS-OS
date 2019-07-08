# My Simple Makefile for bootcode project.
# by Philip R. Simonson
#######################################################################

CFLAGS=-Wall -Werror -Os -march=i686 -ffreestanding -I. -m16
CFLAGS+=-fno-tree-loop-optimize -fomit-frame-pointer

SRCDIR=$(shell pwd)

VERSION=0.1
BASENAM=$(shell basename $(SRCDIR))
TARNAME=$(BASENAM)-$(VERSION).tgz

.PHONY: all disk clean run distclean dist
all: boot.bin command.bin

%.c.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

boot.bin: boot.c.o disk.c.o
	$(LD) $(LDFLAGS) -static -Tboot.ld -melf_i386 -nostdlib \
	--nmagic -o boot.elf $^ && objcopy -O binary boot.elf $@

command.bin: command.c.o io.c.o time.c.o shell.c.o
	$(LD) $(LDFLAGS) -static -Tcommand.ld -melf_i386 -nostdlib \
	--nmagic -o command.elf $^ && objcopy -O binary command.elf $@

disk: boot.bin command.bin
	dd if=/dev/zero of=floppy.img bs=1024 count=1440
	dd if=boot.bin of=floppy.img bs=1 count=512 conv=notrunc
	dd if=command.bin of=floppy.img seek=512 bs=1 conv=notrunc

clean:
	rm -f floppy.img *~ *.elf *.bin *.o

run:
	qemu-system-i386 -soundhw pcspk -fda floppy.img -boot a

distclean: clean

dist:
	@echo Compressing $(TARNAME)...
	@cd .. && tar --exclude=.git -cf - ./$(BASENAM) | \
	gzip -9 > ./$(TARNAME) && echo Compression done! || \
	echo Compression failed.
