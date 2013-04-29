SECTION		.data

digits:		db	"0123456789abcdef"

SECTION		.bss

SECTION		.text

		global	StringAppend
		global	StringAppendChar
		global	StringTail
		global	StringFromCstr
		global	printString
		global	toHex
		global	toDec
		global	toDecSigned

; Prints a string to stdout
;
; @param esi The source address of the string.
printString:
		mov	eax, 4		; System-call "sys_write"
		mov	ebx, 1		; file-descriptor "stdout"
		mov	ecx, esi
		add	ecx, 2		; skip length information
		xor	edx, edx
		mov	dx, [esi]	; length of data
		int	80H

		ret


; Converts a null terminated string into a length defined string
;
; @param esi The source string (null terminated)
; @param edi The destination string
StringFromCstr:
		lea	edi, [edi + 2]	; data portion
		lea	ebx, [edi]	; save address
.convert:
		lodsb			; [esi++] => al
		cmp	al, 0
		je	.done

		stosb			; al => [edi++]
		jmp	.convert
.done:
		mov	eax, edi
		sub	eax, ebx	; number of bytes copied
		mov	[ebx-2], ax

		ret

; Returns the remaining string, starting at a specific index
;
; @param esi The source string
; @param ax  The index in the source string where to start
; @param edi The destination string
StringTail:

; Number of bytes to move
		xor	ecx, ecx
		mov	cx, [esi]	; length of source string
		sub	cx, ax		; number of bytes to move
		jbe	.end		; <= 0, then no bytes to move

; Source address
		lea	esi, [esi+2]	; start of source string data
		xor	ebx, ebx
		mov	bx, ax
		add	esi, ebx	; start index

; Adjust length of destination string
		mov	[edi], ecx	; new result string length

; Destination address
		lea	edi, [edi+2]	; start of destination string

; Copy the bytes
		rep movsb

.end:
		ret


; Appends one string to another
;
; @param esi The source string
; @param edi The destination string
StringAppend:

; Number of bytes to move
		xor	ecx, ecx
		mov	cx, [esi]	; length of source string

; Source address
		lea	esi, [esi+2]	; start of source string data

; Adjust length of destination string
		xor	ebx, ebx
		mov	bx, [edi]	; length of destination string
		add	[edi], ecx	; new result string length

; Destination address
		lea	edi, [edi+ebx+2]; end of destination string

; Copy the bytes
		rep movsb

		ret

; Appends a character to a string
;
; @param al The character to append
; @param edi The destination string
StringAppendChar:
		xor	ebx, ebx
		mov	bx, [edi]	; length of destination string

		inc	word [edi]	; new length of result string

		lea	edi, [edi+ebx+2]; end of destination string
		mov	byte [edi], al	; append character to string

		ret

; Reverses a given String
;
; @param esi The string to reverse
StringReverse:
		xor	ecx, ecx
		mov	cx, [esi]	; get string length
		lea	edi, [esi+1+ecx]; points to last character in string
		lea	esi, [esi+2]	; points to first character in string

.doReverse:
		cmp	edi, esi
		jbe	.done

		; exchange the two characters
		mov	al, [esi]
		mov	ah, [edi]
		mov	[edi], al
		mov	[esi], ah

		dec	edi
		inc	esi
		jmp	.doReverse
.done:
		ret


; Format a hexadezimal notation of a 32 bit number
;
; @param eax	The 32 bit number to print
; @param edi    The destination buffer to retrieve the result string.
;               Needs to have a size of at least 10 bytes (2x size + 8 characters).
toHex:
		mov	word [edi], 8		; result string has fixed length
		add	edi,2
		mov	ecx, 8

.convert:
		rol	eax, 4
		mov	ebx,eax
		and	ebx,0x0000000f
		mov	bl, [digits + ebx]
		mov	byte [edi], bl

		inc	edi
		loop	.convert

		ret


; Converts a 32 bit number into a decimal string
;
; @param eax	The 32 bit number to convert
; @param edi    The destination buffer which receives the string

toDec:
		xor	ecx, ecx		; count = 0
		mov	ebx, 10			; divisor

.convert:
; EAX / 10 => EAX, Remainder in EDX
		xor	edx, edx
		div	ebx		

; Store digit on stack
		push	edx			; save digit on stack
		inc	ecx			; count++

		cmp	eax, 0
		jne	.convert

		mov	word [edi], cx		; store string length
		lea	edi, [edi + 2]		; edi = data portion of the string

.createString:
		pop	edx			; next digit

		mov	bl, [digits + edx]	; get character
		mov	byte [edi], bl		; store character in buffer
		inc	edi			; next location in buffer -

		loop	.createString

		ret


; Print out the signed decimal notation of a 32 bit number
;
; @param eax	The signed 32 bit number to print
; @param edi    The destination buffer for the result string
toDecSigned:
		lea	edi, [edi + 2]	; move to data portion of the string
		mov	esi, edi	; remember edi - needed later for the string length

; check sign flag, store "-" if necessary and adjust value
		test	eax, 0x80000000
		jz	.convert
		mov	byte [edi], '-'
		inc	edi

		neg	eax

.convert:
; EAX / 10 => EAX, Remainder in EDX
		xor	edx, edx
		mov	ebx, 10
		div	ebx		

; Store next character in buffer
		mov	bl, [digits + edx]	; get character
		mov	byte [edi], bl		; store character in buffer
		inc	edi

		cmp	eax, 0
		jne	.convert

; Now, esi=start of string (first character OR sign), edi=behind last string character
		mov	eax, edi
		sub	eax, esi		; calculate string length
		mov	[esi-2], ax		; store string length

; Reverse the string
		dec	edi

		cmp	byte [esi], '-'
		jne	.reverse
		inc	esi

.reverse:
		; exchange two characters
		mov	al, [esi]
		mov	ah, [edi]
		mov	[edi], al
		mov	[esi], ah

		dec	edi
		inc	esi

		cmp	edi, esi
		ja	.reverse

		ret
