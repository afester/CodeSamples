SECTION		.data

sep1:		dw	2
		db	": "
sep2:		dw	2
		db	"|", 10

SECTION		.bss

buffer:		resb	100
hexStr:		resb	20
hexStr2:	resb	20


SECTION		.text

		extern	StringAppend
		extern	StringAppendChar
		extern	StringTail
		extern	StringFromCstr
		extern	printString
		extern	toHex
		extern	toDec
		extern	toDecSigned

		global	_start
_start:
		mov	ecx, [esp]	; argc
		lea	esi, [esp+4]	; argv[0]

.dumpArgs:
		push	ecx
		push	esi

		mov	esi, [esi]
		lea	edi, [buffer]
		call	StringFromCstr

		mov	al, 10
		lea	edi, [buffer]
		call	StringAppendChar

		lea	esi, [buffer]
		call	printString

		pop	esi
		lea	esi, [esi+4]
		pop	ecx
		loop	.dumpArgs


; esi: should be at null ptr
; *****************************************************************************

.dumpEnv:
		lea	esi, [esi+4]	; (next) env variable address address

		mov	ecx, [esi]	; address of env variable
		jecxz	.doneEnv

		push	esi

		mov	esi, ecx
		lea	edi, [buffer]
		call	StringFromCstr

		mov	al, 10
		lea	edi, [buffer]
		call	StringAppendChar

		lea	esi, [buffer]
		call	printString

		pop	esi
		jmp	.dumpEnv
.doneEnv:
		mov	eax, 1		; System-call "sys_exit"
		mov	ebx, 0		; Exit-code
		int	80H




;		call	.getEip
;.getEip:
;		pop	ebp

;		mov	ebp, 0x08048000

		mov	ecx, 64
.dumpLine:
		push	ebp
		push	ecx
		call	dumpLine
		pop	ecx
		pop	ebp
		add	ebp, 16
		loop	.dumpLine

;;;;;;;;;;;;;
		mov	eax, 1		; System-call "sys_exit"
		mov	ebx, 0		; Exit-code
		int	80H

dumpLine:
		mov	eax, ebp
		lea	edi, [buffer]	; address of result buffer in edi
		call	toHex

		lea	esi, [sep1]
		lea	edi, [buffer]
		call	StringAppend

		push	ebp
		mov	ecx, 16
.dumpStack:
		push	ecx

		xor	eax, eax
		mov	al, [ebp]
		lea	edi, [hexStr]	; address of result buffer in edi
		call	toHex

		lea	esi, [hexStr]	; source string
		lea	edi, [hexStr2]	; destination string
		mov	ax, 6		; index in source string
		call	StringTail

		lea	esi, [hexStr2]
		lea	edi, [buffer]
		call	StringAppend

		mov	al, ' '
		lea	edi, [buffer]
		call	StringAppendChar

		pop	ecx
		inc	ebp
		loop	.dumpStack

		pop	ebp
;;;;;;;;;;;;;;;;;;
		mov	al, '|'
		lea	edi, [buffer]
		call	StringAppendChar

		mov	ecx, 16
.dumpAscii:
		push	ecx

		xor	eax, eax
		mov	al, [ebp]
		cmp	al , ' '
		jae	.dumpOk
		mov	al, '.'
.dumpOk:
		cmp	al , 128
		jbe	.dumpOk2
		mov	al, '.'

.dumpOk2:
		lea	edi, [buffer]
		call	StringAppendChar

		pop	ecx
		inc	ebp
		loop	.dumpAscii

		lea	edi, [buffer]
		lea	esi, [sep2]
		call	StringAppend

		lea	esi, [buffer]	; source address of the String
		call	printString

		ret
