; ================================================================
; FAT12 Boot Sector for Floppy Diskettes.
; ================================================================
; Written by  : Philip R. Simonson
; Date Written: July 5, 2019
; ================================================================

[bits 16]
[org 0x0]

nop
jmp short _start ; jump past BPB

bpbOEM			db "PRS OS  "
bpbBytesPerSector:	dw 512
bpbSectorsPerCluster:	db 1
bpbReservedSectors:	dw 1
bpbNumberOfFATs:	db 2
bpbRootEntries:		dw 224
bpbTotalSectors:	dw 2880
bpbMedia:		db 0xf0
bpbSectorsPerFAT:	dw 9
bpbSectorsPerTrack:	dw 18
bpbHeadsPerCylinder:	dw 2
bpbHiddenSectors:	dd 0
bpbTotalSectorsBig:	dd 0
bsDriveNumber:		db 0
bsUnused:		db 0
bsExtBootSignature:	db 0x29
bsSerialNumber:		dd 0xa0a1b2b3
bsVolumeLabel:		db "PRS FLOPPY "
bsFileSystem:		db "FAT     "

; ===============================================================
; Main Entry point
; ===============================================================

_start:
	cli				; disable interrupts
	mov ax, 0x07c0
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	; create stack
	mov ax, 0x0000
	mov ss, ax
	mov sp, 0xFFFF
	sti				; enable interrupts

	; display loading message
	mov si, msg_loading
	call print

	load_root:
	; ================================================
	; compute size of root directory and store in "cx"
	; ================================================
	xor cx, cx
	xor dx, dx
	mov ax, 0x0020
	mul WORD [bpbRootEntries]
	div WORD [bpbBytesPerSector]
	xchg ax, cx

	; ====================================================
	; compute location of root directory and store in "ax"
	; ====================================================
	mov al, BYTE [bpbNumberOfFATs]
	mul WORD [bpbSectorsPerFAT]
	add ax, WORD [bpbReservedSectors]
	mov WORD [datasector], ax
	add WORD [datasector], cx

	; read root directory into memory (7C00:0200)
	mov bx, 0x0200
	call read_sectors

	; =====================================================
	; find boot image in root directory
	; =====================================================
	mov cx, WORD [bpbRootEntries]
	mov di, 0x0200
	.loop:
	push cx
	mov cx, 0x000B
	mov si, image_name
	push di
	rep cmpsb		; test for entry match
	pop di
	je load_fat
	pop cx
	add di, 0x0020		; queue next directory entry
	loop .loop
	jmp failure

	; =====================================================
	; load FAT
	; =====================================================
	load_fat:
;	mov si, msg_crlf
;	call print
	mov dx, WORD [di+0x001A]
	mov WORD [cluster], dx

	; compute size of FAT and store in "cx"
	xor ax, ax
	mov al, BYTE [bpbNumberOfFATs]
	mul WORD [bpbSectorsPerFAT]
	mov cx, ax

	; compute location of FAT and store in "ax"
	mov ax, WORD [bpbReservedSectors]

	; read FAT into memory (7C00:0200)
	mov bx, 0x0200
	call read_sectors

	; read image file into memory
;	mov si, msg_crlf
;	call print
	mov ax, 0x0050
	mov es, ax
	mov bx, 0x0000
	push bx

	; ======================================================
	; load image "command.bin"
	; ======================================================
	load_image:
	mov ax, WORD [cluster]
	pop bx
	call clusterlba
	xor cx, cx
	mov cl, BYTE [bpbSectorsPerCluster]
	call read_sectors
	push bx

	; compute next cluster
	mov ax, WORD [cluster]
	mov cx, ax
	mov dx, ax
	shr dx, 0x0001
	add cx, dx
	mov bx, 0x0200
	add bx, cx
	mov dx, WORD [bx]
	test ax, 0x0001
	jnz .odd_cluster
	.even_cluster:
	and dx, 0000111111111111b
	jmp .done
	.odd_cluster:
	shr dx, 0x0004
	.done:
	mov WORD [cluster], dx
	cmp dx, 0x0FF0
	jb load_image
	done:
	mov si, msg_crlf
	call print
	push WORD 0x0050
	push WORD 0x0000
	retf

	failure:
	mov si, msg_failure
	call print
	mov ah, 0x00
	int 16h
	int 19h

; ===============================================================
; Various functions
; ===============================================================

print:
	lodsb
	or al, al
	jz .done
	mov ah, 0eh
	int 10h
	jmp print
	.done:
	ret

read_sectors:
	.main:
	mov di, 5h
	.sectorloop:
	push ax
	push bx
	push cx
	call lbachs
	mov ah, 2h
	mov al, 1h
	mov ch, BYTE [absoluteTrack]
	mov cl, BYTE [absoluteSector]
	mov dh, BYTE [absoluteHead]
	mov dl, BYTE [bsDriveNumber]
	int 13h
	jnc .success
	xor ax, ax
	int 13h
	dec di
	pop cx
	pop bx
	pop ax
	jnz .sectorloop
	int 18h
	.success:
	mov si, msg_progress
	call print
	pop cx
	pop bx
	pop ax
	add bx, WORD [bpbBytesPerSector]
	inc ax
	loop .main
	ret

; convert CHS to LBA
clusterlba:
	sub ax, 0x0002
	xor cx, cx
	mov cl, BYTE [bpbSectorsPerCluster]
	mul cx
	add ax, WORD [datasector]
	ret

; convert LBA to CHS
lbachs:
	xor dx, dx
	div WORD [bpbSectorsPerTrack]
	inc dl
	mov BYTE [absoluteSector], dl
	xor dx, dx
	div WORD [bpbHeadsPerCylinder]
	mov BYTE [absoluteHead], dl
	mov BYTE [absoluteTrack], al
	ret

; data section
; ====================================

absoluteSector db 0x00
absoluteHead db 0x00
absoluteTrack db 0x00

datasector dw 0x0000
cluster dw 0x0000
image_name db "COMMAND BIN",0x00
msg_loading db "Loading shell",0x00
msg_crlf db 0x0a, 0x0d, 0x00
msg_progress db 0x2e, 0x00
msg_failure db "ERROR: Press any key to reboot.", 0x0a, 0x0d, 0x00

times 510-($-$$) db 0
dw 0xaa55
