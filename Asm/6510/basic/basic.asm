; Simple assembly application which can be loaded with LOAD"...",8,1
; and then be called with SYS 49152

        *=$c000
        .byt $00, $c0	; load addr - first two bytes of PRG file

; $c000 (49152) - start of assembly program
start:
	rts		; return to BASIC
