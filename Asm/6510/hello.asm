; Simple application which loads properly with LOAD"...",8
; Contains one basic line which calls the assembly routine starting
; at "start:"

        *=$7ff
        .byt $01, $08	; load addr - first two bytes of PRG file

; $801 - start of basic area (this is the start of the image which is loaded)
basic:
        .byt $0C, $08, $0A, $00, $9E, $20,  $32, $30, $36, $32, $00, $00, $00 ; , $00, $00
;            NextLine  Line Nr   SYS  ' '     2    0    6    2  EOL     EOP   ;  Fill Bytes

; $80e (2062) - start of assembly program
start:

	jsr	fillScreen

	lda	#65
loop:
	sta	53280
	adc	#1
	jmp	loop

fillScreen:
	ldx	#0
	lda	#65
loop1:
	sta	$0400,x
	inx
	bne	loop1
	rts
