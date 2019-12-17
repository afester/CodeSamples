		.686
		.mmx
		.xmm
		.model flat	; , stdcall		; Flat memory model and stdcall method

		.data

HelloStr	dw	5
		db	"Hello"
WorldStr	dw	6
		db	"World!"
MoonStr	db	"Hello Moon!", 0

		.data?

result	db	100 DUP(?)
buffer	db	100 DUP(?)

ebxReg	dq	?
ecxReg	dq	?
edxReg	dq	?

startHigh	dq	?
startLow	dq	?
stopHigh	dq	?
stopLow		dq	?
diffHigh	dq	?
diffLow		dq	?

		.code

		extern	StringAppend : PROC
		extern	StringTail : PROC
		extern	StringAppendChar : PROC
		extern	StringFromCstr : PROC
		extern	printString : PROC
		extern	toHex : PROC
		extern	toDec : PROC
		extern	toDecSigned : PROC

		extrn  _ExitProcess@4 : PROC

		public	main
main	PROC

		xor	eax, eax
		cpuid

		mov	word ptr [result], 12	; length of result string = 0
		mov	dword ptr [result + 2], EBX
		mov	dword ptr [result + 6], EDX
		mov	dword ptr [result + 10], ECX

		lea	edi, [buffer]
		call	toHex			; EAX to buffer

		mov	al, 10
		lea	edi, [result]		; destination string
		call	StringAppendChar

		lea	esi, [result]		; source string
		call	printString

		mov	al, 10
		lea	edi, [buffer]		; destination string
		call	StringAppendChar

		lea	esi, [buffer]		; source string
		call	printString

		mov		eax, 1
		cpuid
		mov		dword ptr [ebxReg], ebx
		mov		dword ptr [ecxReg], ecx
		mov		dword ptr [edxReg], edx

		lea	edi, [buffer]
		call	toHex			; EAX to buffer
		mov	al, 10
		lea	edi, [buffer]		; destination string
		call	StringAppendChar
		lea	esi, [buffer]		; source string
		call	printString

		lea	edi, [buffer]
		mov	eax, dword ptr [ebxReg]
		call	toHex
		mov	al, 10
		lea	edi, [buffer]		; destination string
		call	StringAppendChar
		lea	esi, [buffer]		; source string
		call	printString

				lea	edi, [buffer]
		mov	eax, dword ptr [ecxReg]
		call	toHex
		mov	al, 10
		lea	edi, [buffer]		; destination string
		call	StringAppendChar
		lea	esi, [buffer]		; source string
		call	printString

		lea	edi, [buffer]
		mov	eax, dword ptr [edxReg]
		call	toHex
		mov	al, 10
		lea	edi, [buffer]		; destination string
		call	StringAppendChar
		lea	esi, [buffer]		; source string
		call	printString

; ****************************************************************************


		lea	eax,[result]
		clflush	[eax]

		mov		ecx, dword ptr [result]

		lfence
		rdtsc
		mov		dword ptr [startLow], eax
		mov		dword ptr [startHigh], edx

		mov		ebx, dword ptr [result]
		
		lfence
		rdtsc
		mov		dword ptr [stopLow], eax
		mov		dword ptr [stopHigh], edx

		mov	eax, dword ptr [startHigh]
		lea	edi, [result]
		call	toHex
		mov	al, ':'
		lea	edi, [result]		; destination string
		call	StringAppendChar
		lea	edi, [buffer]
		mov	eax, dword ptr [startLow]
		call	toHex
		lea	edi, [result]		; destination string
		lea	esi, [buffer]		; source string
		call	StringAppend
		mov	al, 10
		lea	edi, [result]		; destination string
		call	StringAppendChar

		mov	eax, dword ptr [stopHigh]
		lea	edi, [buffer]
		call	toHex
		lea	edi, [result]		; destination string
		lea	esi, [buffer]		; source string
		call	StringAppend
		mov	al, ':'
		lea	edi, [result]		; destination string
		call	StringAppendChar
		lea	edi, [buffer]
		mov	eax, dword ptr [stopLow]
		call	toHex
		lea	edi, [result]		; destination string
		lea	esi, [buffer]		; source string
		call	StringAppend

		mov		eax, dword ptr [stopLow]
		sub		eax, dword ptr [startLow]
		mov		dword ptr [diffLow], eax
		mov		eax, dword ptr [stopHigh]
		sbb		eax, dword ptr [startHigh]
		mov		dword ptr [diffHigh], eax

;		mov	al, ' '
;		lea	edi, [result]		; destination string
;		call	StringAppendChar
;		mov	eax, dword ptr [diffHigh]
;		lea	edi, [buffer]
;		call	toHex
;		lea	edi, [result]		; destination string
;		lea	esi, [buffer]		; source string
;		call	StringAppend
;		mov	al, ':'
;		lea	edi, [result]		; destination string
;		call	StringAppendChar
;		lea	edi, [buffer]
;		mov	eax, dword ptr [diffLow]
;		call	toHex
;		lea	edi, [result]		; destination string
;		lea	esi, [buffer]		; source string
;		call	StringAppend

		mov	al, ' '
		lea	edi, [result]		; destination string
		call	StringAppendChar
		mov	al, '('
		lea	edi, [result]		; destination string
		call	StringAppendChar

		mov	eax, dword ptr [diffLow]
		lea	edi, [buffer]
		call	toDec
		lea	edi, [result]		; destination string
		lea	esi, [buffer]		; source string
		call	StringAppend
		mov	al, ')'
		lea	edi, [result]		; destination string
		call	StringAppendChar

		mov	al, 10
		lea	edi, [result]		; destination string
		call	StringAppendChar

		lea	esi, [result]		; source string
		call	printString

;*************************************************************************

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
