SECTION		.data

HelloStr:	dw	5
		db	"Hello"
WorldStr:	dw	6
		db	"World!"
MoonStr:	db	"Hello Moon!", 0

SECTION		.bss

result		resb	100
buffer		resb	100

SECTION		.text

		extern	StringAppend
		extern	StringTail
		extern	StringAppendChar
		extern	StringFromCstr
		extern	printString
		extern	toHex
		extern	toDec
		extern	toDecSigned

		global	_start
_start:
		mov	word [result], 0	; length of result string = 0

; result = result + "Hello"
		lea	edi, [result]		; destination string
		lea	esi, [HelloStr]		; source string
		call	StringAppend

; result = result + ' '
		mov	al, ' '
		lea	edi, [result]		; destination string
		call	StringAppendChar

; result = result + "World!"
		lea	edi, [result]		; destination string
		lea	esi, [WorldStr]		; source string
		call	StringAppend

; result = result + '\n'
		mov	al, 10
		lea	edi, [result]		; destination string
		call	StringAppendChar

; print result
		lea	esi, [result]		; source string
		call	printString



		lea	esi, [result]		; Source string
		mov	ax, 6			; Index in source string
		lea	edi, [buffer]		; destination string
		call	StringTail

; print result
		lea	esi, [buffer]		; source string
		call	printString

;******************************************************************************

; Convert 1234567890 into a string
		mov	eax, 1234567890
		lea	edi, [result]
		call	toDec

; result = result + '\n'
		mov	al, 10
		lea	edi, [result]		; destination string
		call	StringAppendChar

		lea	esi, [result]		; source string
		call	printString

; Convert -1234567890 into a string
		mov	eax, -1234567890
		lea	edi, [result]
		call	toDecSigned

; result = result + '\n'
		mov	al, 10
		lea	edi, [result]		; destination string
		call	StringAppendChar

; print result
		lea	esi, [result]		; source string
		call	printString

; Convert 1234567890 into a hex string
		mov	eax, 1234567890
		lea	edi, [result]
		call	toHex

; result = result + '\n'
		mov	al, 10
		lea	edi, [result]		; destination string
		call	StringAppendChar

; print result
		lea	esi, [result]		; source string
		call	printString

; Get a String from a null terminated string
		lea	esi, [MoonStr]
		lea	edi, [result]
		call	StringFromCstr

		mov	al, 10
		lea	edi, [result]		; destination string
		call	StringAppendChar

		lea	esi, [result]		; source string
		call	printString

		mov	eax, 1			; System-call "sys_exit"
		mov	ebx, 0			; Exit-code
		int	80H
