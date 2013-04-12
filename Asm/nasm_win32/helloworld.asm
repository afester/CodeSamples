SECTION		.data

Message:	db      'Hello World!', 10
MsgLen:         equ     $-Message

SECTION         .bss

SECTION         .text

		extern  _GetStdHandle@4
		extern  _WriteFile@20
		extern  _ExitProcess@4

		global _main
_main:
		; DWORD  bytes;    
		mov     ebp, esp
		sub     esp, 4

		; hStdOut = GetstdHandle( STD_OUTPUT_HANDLE)
		push    -11
		call    _GetStdHandle@4
		mov     ebx, eax    

		; WriteFile( hstdOut, message, length(message), &bytes, 0);
		push    0			; lpOverlapped
		lea     eax, [ebp-4]
		push    eax			; Number of bytes written
		push    MsgLen			; Number of bytes to write
		push    Message			; buffer
		push    ebx			; file handle
		call    _WriteFile@20

		; ExitProcess(0)
		push    0
		call    _ExitProcess@4

		; never here
		hlt
