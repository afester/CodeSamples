.data

.bss

.text

.globl	_start

_start:
                fsave   (%esp)

		movl	$1, %eax			# System-call "sys_exit"
		movl 	$0, %ebx			# Exit-code
		int     $0x80
