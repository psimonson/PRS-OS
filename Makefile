# My Simple Makefile for bootcode project.
# by Philip R. Simonson
#######################################################################

SRCDIR=$(shell pwd)
USER  =$(shell whoami)

CFLAGS=-std=gnu89 -Wall -Werror -s -Os -march=i686 -ffreestanding -I. -m16 -pedantic
CFLAGS+=-fno-asynchronous-unwind-tables -fno-pic -fno-pie -fno-builtin
CFLAGS+=-fomit-frame-pointer -ffunction-sections -fdata-sections -fno-ident

LDFLAGS=-static -s -Os -m elf_i386 -no-pie -nostartfiles --nmagic
LDFLAGS+=--gc-sections# --oformat binary

# ===============================
# COMMAND.BIN OBJECTS
# ===============================
CMDOBJS=command.c.o io.c.o time.c.o string.c.o shell.c.o disk.c.o fat12.c.o

SOURCES=$(wildcard *.c)
HEADERS=$(wildcard *.h)
DISKFILES=$(SOURCES) $(HEADERS)

VERSION=0.1
BASENAM=$(shell basename $(SRCDIR))
TARNAME=$(BASENAM)-$(VERSION).tgz

.PHONY: all disk disk2 clean run distclean dist
all: boot.bin command.bin #makeboot

%.c.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -fno-PIC -c $< -o $@

makeboot: makeboot.c
	$(CC) -std=c89 -Wall -Wextra -Werror -I. $(shell pkg-config --cflags prs) \
	-o $@ $^ $(shell pkg-config --libs prs)

boot.bin: boot.asm
	nasm -f bin -o $@ $^

command.bin: $(CMDOBJS)
	$(LD) -Tcommand.ld $(LDFLAGS) -o command.elf $^
	strip --strip-unneeded -R .comment -R .note -R .gnu.version command.elf
	objcopy -O binary command.elf $@

disk: all
ifneq (,$(wildcard ./floppy.img))
	rm ./floppy.img
endif
	sudo mkfs.fat -F12 -I -C "floppy.img" 1440
	sudo mount -t vfat -o loop floppy.img /mnt
	sudo cp command.bin /mnt/command.bin
	sudo umount -R /mnt
	sudo dd if=boot.bin of=floppy.img bs=1 count=512 conv=notrunc
	sudo chown $(USER):users floppy.img

disk2: all
ifneq (,$(wildcard ./floppy.img))
	rm ./floppy.img
endif
	sudo mkfs.fat -F12 -I -C "floppy.img" 1440
	sudo mount -t vfat -o loop floppy.img /mnt
	sudo cp command.bin /mnt
	sudo cp $(DISKFILES) /mnt
	sudo umount -R /mnt
	sudo dd if=boot.bin of=floppy.img bs=1 count=512 conv=notrunc
	sudo chown $(USER):users floppy.img

clean:
	rm -f floppy.img *~ *.elf *.bin *.o

run:
	qemu-system-i386 -soundhw pcspk -fda floppy.img -boot a
#	bochs

distclean: clean

dist:
	@echo Compressing $(TARNAME)...
	@cd .. && tar --exclude=.git -cf - ./$(BASENAM) | \
	gzip -9 > ./$(TARNAME) && echo Compression done! || \
	echo Compression failed.
