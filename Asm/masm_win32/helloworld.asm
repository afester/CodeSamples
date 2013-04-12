		.586				; Pentium Processor Instruction Set
		.model flat,stdcall		; Flat memory model and stdcall method

		.data

Message		db      'Hello World!', 10
MsgLen		equ     $-Message

		.data?

		.code

		extrn  GetStdHandle@4 : PROC
		extrn  WriteFile@20 : PROC
		extrn  ExitProcess@4 : PROC

_main		PROC
		; DWORD  bytes;    
		mov     ebp, esp
		sub     esp, 4

		; hStdOut = GetstdHandle( STD_OUTPUT_HANDLE)
		push    -11
		call    GetStdHandle@4
		mov     ebx, eax    

		; WriteFile( hstdOut, message, length(message), &bytes, 0);
		push    0			; lpOverlapped
		lea     eax, [ebp-4]
		push    eax			; Number of bytes written
		pushd 	MsgLen			; Number of bytes to write
		push    offset Message		; buffer
		push    ebx			; file handle
		call    WriteFile@20

		; ExitProcess(0)
		push    0
		call    ExitProcess@4

		; never here
		hlt
_main		ENDP

		end
