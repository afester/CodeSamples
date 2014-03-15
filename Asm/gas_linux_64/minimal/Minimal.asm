.globl	_start
_start:
    movq $1, %rax
    cpuid

    mov	$60, %rax	# System-call "sys_exit"
    mov	$0, %rdi	# exit code 0
    syscall
