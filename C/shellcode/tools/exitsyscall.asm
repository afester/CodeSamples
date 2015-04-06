Section  .text
    global _start

_start:

;    mov ebx,0
    xor ebx, ebx

;    mov eax,1
    xor eax, eax
    inc eax

    int 0x80
