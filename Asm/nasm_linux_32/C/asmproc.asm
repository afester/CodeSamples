SECTION		.data

Message:	db	"Hello World!", 10
MsgLen:		equ	$-Message

SECTION		.bss

SECTION		.text

		global main
main:
		mov	eax, 4		; System-call "sys_write"
		mov	ebx, 1		; file-descriptor "stdout"
		mov	ecx, Message	; Data to write
		mov	edx, MsgLen	; length of data
		int	80H

		ret
