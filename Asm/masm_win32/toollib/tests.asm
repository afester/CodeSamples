		.586				; Pentium Processor Instruction Set
		.model flat	; , stdcall		; Flat memory model and stdcall method

		.data

		Messag		dw 12
					db 'Hello Moon!', 10
;MsgLen		equ     $-Message

HelloStr	dw	5
		db	"Hello"
WorldStr	dw	6
		db	"World!"
MoonStr	db	"Hello Moon!", 0

		.data?

result		db	100 DUP(?)
buffer		db	100 DUP(?)

		.code

		extern	StringAppend : PROC
		extern	StringTail : PROC
		extern	StringAppendChar : PROC
		extern	StringFromCstr : PROC
		extern	printString : PROC
		extern	toHex : PROC
		extern	toDec : PROC
		extern	toDecSigned : PROC

		extrn  _GetStdHandle@4 : PROC
		extrn  _WriteFile@20 : PROC
		extrn  _ExitProcess@4 : PROC

		public	main
main	PROC

		mov	word ptr [result], 0	; length of result string = 0

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

; ExitProcess(0)
		push    0
		call    _ExitProcess@4

		; never here
		hlt

main	ENDP

		END
