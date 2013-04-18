SECTION		.data

Message:	db	"Hello World!", 10
MsgLen:		equ	$-Message

SECTION		.bss

SECTION		.text

		global	_start
_start:
		nop

; The 64 bit kernel interface uses %rdi, %rsi, %rdx, %r10, %r8 and %r9.
		mov	rax, 1		; System-call "sys_write"
		mov	rdi, 1		; file-descriptor "stdout"
		mov	rsi, Message	; Data to write
		mov	rdx, MsgLen	; length of data
		syscall

		mov	rax, 60		; System-call "sys_exit"
		mov	rdi, 0		; exit code 0
		syscall
