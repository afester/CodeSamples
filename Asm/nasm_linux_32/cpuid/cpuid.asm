SECTION		.data

newline:	dw	1		; length
		db	10		; contents

SECTION		.bss

buffer:		resb	20
regax:		resd	1
regbx:		resd	1
regcx:		resd	1
regdx:		resd	1

SECTION		.text

		extern	toHex
		extern	printString
		extern	StringAppend

		global	_start
_start:
		cpuid
		mov	[regax], eax
		mov	[regbx], ebx
		mov	[regcx], ecx
		mov	[regdx], edx

                mov     eax, [regax]    ; input value in eax
                lea     edi, [buffer]   ; address of result buffer in edi
                call    toHex           ; convert number to hex string

                lea     esi, [newline]  ; source string
                lea     edi, [buffer]   ; destination string
                call    StringAppend

                lea     esi, [buffer]   ; source address of the String
                call    printString


                mov     eax, [regbx]    ; input value in eax
                lea     edi, [buffer]   ; address of result buffer in edi
                call    toHex           ; convert number to hex string

                lea     esi, [newline]  ; source string
                lea     edi, [buffer]   ; destination string
                call    StringAppend

                lea     esi, [buffer]   ; source address of the String
                call    printString


                mov     eax, [regcx]    ; input value in eax
                lea     edi, [buffer]   ; address of result buffer in edi
                call    toHex           ; convert number to hex string

                lea     esi, [newline]  ; source string
                lea     edi, [buffer]   ; destination string
                call    StringAppend

                lea     esi, [buffer]   ; source address of the String
                call    printString


                mov     eax, [regdx]    ; input value in eax
                lea     edi, [buffer]   ; address of result buffer in edi
                call    toHex           ; convert number to hex string

                lea     esi, [newline]  ; source string
                lea     edi, [buffer]   ; destination string
                call    StringAppend

                lea     esi, [buffer]   ; source address of the String
                call    printString

		mov	eax, 1		; System-call "sys_exit"
		mov	ebx, 0		; Exit-code
		int	80H
