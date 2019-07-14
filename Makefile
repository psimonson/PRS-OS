# My Simple Makefile for bootcode project.
# by Philip R. Simonson
#######################################################################

CFLAGS=-Wall -Werror -Os -march=i686 -ffreestanding -I. -m32
SRCDIR=$(shell pwd)

VERSION=0.1
BASENAM=$(shell basename $(SRCDIR))
TARNAME=$(BASENAM)-$(VERSION).tgz

.PHONY: all disk clean run distclean dist
all: boot.bin command.bin makeboot

%.c.o: %.c
	$(CC) $(CFLAGS) -fno-PIC -c $< -o $@

makeboot: makeboot.c
	$(CC) -std=c89 -Wall -Wextra -Werror -I. $(shell pkg-config --cflags prs) -o $@ $^ \
	$(shell pkg-config --libs prs)

boot.bin: boot.asm
	nasm -f bin -o $@ $^

command.bin: command.c.o io.c.o time.c.o shell.c.o string.c.o disk.c.o fat12.c.o
	$(LD) $(LDFLAGS) -no-PIE -static -e main -Ttext=0x0000 -R.note -R.comment \
	-melf_i386 -nostdlib --nmagic --oformat binary -o $@ $^

disk: all
	dd if=/dev/zero of=floppy.img bs=1024 count=1440
	sudo losetup /dev/loop0 floppy.img
	sudo mkfs.vfat -F12 /dev/loop0
	sudo mount /dev/loop0 /mnt
	sudo cp command.bin /mnt
	sudo umount /dev/loop0
	sudo ./makeboot /dev/loop0 || sudo losetup -d /dev/loop0
	sudo losetup -d /dev/loop0
#	dd if=boot.bin of=floppy.img bs=1 count=512 conv=notrunc

clean:
	rm -f floppy.img *~ *.bin *.o makeboot

run:
	qemu-system-i386 -soundhw pcspk -fda floppy.img -boot a

distclean: clean

dist:
	@echo Compressing $(TARNAME)...
	@cd .. && tar --exclude=.git -cf - ./$(BASENAM) | \
	gzip -9 > ./$(TARNAME) && echo Compression done! || \
	echo Compression failed.
