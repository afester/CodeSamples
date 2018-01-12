		.586				; Pentium Processor Instruction Set
		.model flat  ; ,stdcall		; Flat memory model and stdcall method

		.data

digits	db	'0123456789abcdef'

		.code

		extrn  _GetStdHandle@4 : PROC
		extrn  _WriteFile@20 : PROC
		extrn  _ExitProcess@4 : PROC

; Prints a string to stdout
;
; @param esi The source address of the string.
printString	PROC
		; DWORD  bytes;    
		mov     ebp, esp
		sub     esp, 4

		; hStdOut = GetstdHandle( STD_OUTPUT_HANDLE)
		push    -11
		call    _GetStdHandle@4
		mov     ebx, eax    

		; WriteFile( hstdOut, message, length(message), &bytes, 0);
		push    0			; lpOverlapped (4 bytes)

		lea     eax, [ebp-4]
		push    eax			; Number of bytes written (4 bytes)

		xor		eax, eax
		mov		ax, [esi]
		push 	eax			; Number of bytes to write (4 bytes)

		add		esi,2		; skip length information
		push    esi			; buffer (4 bytes)

		push    ebx			; file handle (4 bytes)
		call    _WriteFile@20

		add		esp, 4
		ret
printString	ENDP

; Converts a null terminated string into a length defined string
;
; @param esi The source string (null terminated)
; @param edi The destination string
StringFromCstr	PROC
		lea	edi, [edi + 2]	; data portion
		lea	ebx, [edi]	; save address
convert:
		lodsb			; [esi++] => al
		cmp	al, 0
		je	done

		stosb			; al => [edi++]
		jmp	convert
done:
		mov	eax, edi
		sub	eax, ebx	; number of bytes copied
		mov	[ebx-2], ax

		ret
StringFromCstr	ENDP


; Returns the remaining string, starting at a specific index
;
; @param esi The source string
; @param ax  The index in the source string where to start
; @param edi The destination string
StringTail	PROC

; Number of bytes to move
		xor	ecx, ecx
		mov	cx, [esi]	; length of source string
		sub	cx, ax		; number of bytes to move
		jbe	goEnd		; <= 0, then no bytes to move

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

goEnd:
		ret
StringTail	ENDP

; Appends one string to another
;
; @param esi The source string
; @param edi The destination string
StringAppend	PROC

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
StringAppend	ENDP


; Appends a character to a string
;
; @param al The character to append
; @param edi The destination string
StringAppendChar	PROC
		xor	ebx, ebx
		mov	bx, [edi]	; length of destination string

		inc	word ptr [edi]	; new length of result string

		lea	edi, [edi+ebx+2]; end of destination string
		mov	[edi], al	; append character to string

		ret
StringAppendChar	ENDP

; Reverses a given String
;
; @param esi The string to reverse
StringReverse	PROC
		xor	ecx, ecx
		mov	cx, [esi]	; get string length
		lea	edi, [esi+1+ecx]; points to last character in string
		lea	esi, [esi+2]	; points to first character in string

doReverse:
		cmp	edi, esi
		jbe	done

		; exchange the two characters
		mov	al, [esi]
		mov	ah, [edi]
		mov	[edi], al
		mov	[esi], ah

		dec	edi
		inc	esi
		jmp	doReverse
done:
		ret
StringReverse	ENDP


; Format a hexadezimal notation of a 32 bit number
;
; @param eax	The 32 bit number to print
; @param edi    The destination buffer to retrieve the result string.
;               Needs to have a size of at least 10 bytes (2x size + 8 characters).
toHex	PROC
		mov	word ptr [edi], 8		; result string has fixed length
		add	edi,2
		mov	ecx, 8

convert:
		rol	eax, 4
		mov	ebx, eax
		and	ebx, 0000000fh
		mov	bl, [digits + ebx]
		mov	[edi], bl

		inc	edi
		loop	convert

		ret
toHex	ENDP

; Converts a 32 bit number into a decimal string
;
; @param eax	The 32 bit number to convert
; @param edi    The destination buffer which receives the string

toDec	PROC
		xor	ecx, ecx		; count = 0
		mov	ebx, 10			; divisor

convert:
; EAX / 10 => EAX, Remainder in EDX
		xor	edx, edx
		div	ebx		

; Store digit on stack
		push	edx			; save digit on stack
		inc	ecx			; count++

		cmp	eax, 0
		jne	convert

		mov	[edi], cx		; store string length
		lea	edi, [edi + 2]		; edi = data portion of the string

createString:
		pop	edx			; next digit

		mov	bl, [digits + edx]	; get character
		mov	[edi], bl		; store character in buffer
		inc	edi			; next location in buffer -

		loop	createString

		ret
toDec	ENDP

; Print out the signed decimal notation of a 32 bit number
;
; @param eax	The signed 32 bit number to print
; @param edi    The destination buffer for the result string
toDecSigned	PROC
		lea	edi, [edi + 2]	; move to data portion of the string
		mov	esi, edi	; remember edi - needed later for the string length

; check sign flag, store "-" if necessary and adjust value
		test	eax, 80000000h
		jz	convert
		mov	byte ptr [edi], '-'
		inc	edi

		neg	eax

convert:
; EAX / 10 => EAX, Remainder in EDX
		xor	edx, edx
		mov	ebx, 10
		div	ebx		

; Store next character in buffer
		mov	bl, [digits + edx]	; get character
		mov	[edi], bl		; store character in buffer
		inc	edi

		cmp	eax, 0
		jne	convert

; Now, esi=start of string (first character OR sign), edi=behind last string character
		mov	eax, edi
		sub	eax, esi		; calculate string length
		mov	[esi-2], ax		; store string length

; Reverse the string
		dec	edi

		cmp	byte ptr [esi], '-'
		jne	reverse
		inc	esi

reverse:
		; exchange two characters
		mov	al, [esi]
		mov	ah, [edi]
		mov	[edi], al
		mov	[esi], ah

		dec	edi
		inc	esi

		cmp	edi, esi
		ja	reverse

		ret
toDecSigned	ENDP

	END