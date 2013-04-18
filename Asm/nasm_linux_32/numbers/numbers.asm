SECTION		.data

Message:	db	10
MsgLen:		equ	$-Message

newline:	dw	1		; length
		db	10		; contents

input:		dq	-12345
;input:		dq	-128
;input:		dq	0xabcdef12

SECTION		.bss

buffer:		resb	20

SECTION		.text

		extern	StringAppend
		extern	printString
		extern	toHex
		extern	toDec
		extern	toDecSigned

		global	_start
_start:
		mov	bx, 12345
		movsx	eax, bx
		lea	edi, [buffer]	; address of result buffer in edi
		call	toDecSigned

		lea	esi, [newline]	; source string
		lea	edi, [buffer]	; destination string
		call	StringAppend

		lea	esi, [buffer]	; source address of the String
		call	printString

		mov	eax, [input]	; input value in eax
		lea	edi, [buffer]	; address of result buffer in edi
		call	toHex		; convert number to hex string

		lea	esi, [newline]	; source string
		lea	edi, [buffer]	; destination string
		call	StringAppend

		lea	esi, [buffer]	; source address of the String
		call	printString

		mov	eax, [input]
		lea	edi, [buffer]
		call	toDec

		lea	esi, [newline]	; source string
		lea	edi, [buffer]	; destination string
		call	StringAppend

		lea	esi, [buffer]	; source address of the String
		call	printString

		mov	eax, [input]
		lea	edi, [buffer]
		call	toDecSigned

		lea	esi, [newline]	; source string
		lea	edi, [buffer]	; destination string
		call	StringAppend

		lea	esi, [buffer]	; source address of the String
		call	printString

		mov	eax, 1		; System-call "sys_exit"
		mov	ebx, 0		; Exit-code
		int	80H
