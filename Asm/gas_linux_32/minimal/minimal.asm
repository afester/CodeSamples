.text
.global _start
_start:
        movl $10, %eax
;        pushl %eax
        movl $1, %eax
        int 0x80
.data
.equ exit, 0x80
