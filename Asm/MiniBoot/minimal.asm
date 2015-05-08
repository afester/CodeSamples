SECTION		.data


SECTION		.bss

SECTION		.text

;                extern  main

		org	0x7c00
;                global  _start
;_start:


		cli
		call	getIp			; push  eip
getIp:		push	cs
		push	ds
		push	es
		push	sp	

		mov	bx, cs
		mov	ds, bx

; SP
		xor	eax, eax
		pop	ax
		lea	si, [SPText]
		mov	bx,0			; Line 1
		call	printReg

; ES
		xor	eax, eax
		pop	ax
		lea	si, [ESText]
		mov	bx,160			; Line 2
		call	printReg

; DS
		xor	eax, eax
		pop	ax
		lea	si, [DSText]
		mov	bx,320			; Line 3
		call	printReg

; CS
		xor	eax, eax
		pop	ax
		lea	si, [CSText]
		mov	bx,480			; Line 4
		call	printReg

; IP
		xor	eax, eax
		pop	ax
		lea	si, [IPText]
		mov	bx,640			; Line 5
		call	printReg

;		call	main

		hlt

printReg:
		push	bx
		lea	di, [buffer]
		call	toHex

		lea	di, [result]
		mov	word [di],0
		call	StringAppend

		lea	di, [result]
		lea	si, [buffer]
		call	StringAppend

		lea	si, [result]
		pop	di
		call	printString

		ret


; Prints a string to the screen
;
; @param ds:si The source address of the string.
; @param di The destination address of the string
printString:
		mov	bx, 0xb800
		mov	es, bx

		mov	ah, 0x61	; attribute
		mov	cx, word [si]	; string length
		add	si, 2

		cld
.print:
		lodsb			; [si++] => al
		stosw			; ax => [di++]
		loop	.print

                ret


; Appends one string to another
;
; @param esi The source string
; @param edi The destination string
StringAppend:

		mov	bx, cs
		mov	es, bx
; Number of bytes to move
;                xor     ecx, ecx
                mov     cx, [si]       ; length of source string

; Source address
                lea     si, [si+2]    ; start of source string data

; Adjust length of destination string
;                xor     ebx, ebx
                mov     bx, [edi]       ; length of destination string
                add     [edi], cx      ; new result string length

; Destination address
                lea     di, [di+bx+2]; end of destination string

; Copy the bytes
                rep movsb

                ret


; Format a hexadezimal notation of a 32 bit number
;
; @param eax    The 32 bit number to print
; @param edi    The destination buffer to retrieve the result string.
;               Needs to have a size of at least 10 bytes (2x size + 8 characters).
toHex:
                mov     word [di], 8           ; result string has fixed length
                add     di, 2
                mov     ecx, 8

.convert:
                rol     eax, 4
                mov     ebx,eax
                and     ebx,0x0000000f
                mov     bl, [digits + ebx]
                mov     byte [di], bl

                inc     edi
                loop    .convert

                ret

IPText:		dw 6
		db "IP: 0x"
SPText:		dw 6
		db "SP: 0x"
ESText:		dw 6
		db "ES: 0x"
DSText:		dw 6
		db "DS: 0x"
CSText:		dw 6
		db "CS: 0x"
digits:         db      "0123456789abcdef"
buffer:		resb	20
result:		resb	20
