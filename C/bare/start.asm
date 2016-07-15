	.file	"start.asm"
	
	.text
	.globl	_start
	.type	main, @function
_start:
; Note: This is real mode. Addresses are restricted to 20 bits
; (even though we have 32 bit registers available),
; and segment registers need to be setup properly!
	cli		
	call	main	
	hlt		
