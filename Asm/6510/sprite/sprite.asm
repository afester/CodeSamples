; Simple application which loads properly with LOAD"...",8
; Contains one basic line which calls the assembly routine starting
; at "start:"

;        *=$7ff
;        byte $01, $08   ; load addr - first two bytes of PRG file

; 10 SYS (2064)


Incasm "macros.asm"



*=$0801

        BYTE    $0E, $08, $0A, $00, $9E, $20, $28,  $32, $30, $36, $34, $29, $00, $00, $00

        ; sprptr0 = 2040

; $801 - start of basic area (this is the start of the image which is loaded)
;         *=$801
;basic:
;        byte $0C, $08, $0A, $00, $9E, $20,  $32, $30, $36, $32, $00, $00, $00 ; , $00, $00
;            NextLine  Line Nr   SYS  ' '     2    0    6    2  EOL     EOP   ;  Fill Bytes

; $80e (2062) - start of assembly program
start:
        sei        ;disable maskable IRQs

        setBgColor DARKGREY
        clrscr

        lda #$7f
        sta $dc0d  ;disable timer interrupts which can be generated by the two CIA chips
        sta $dd0d  ;the kernal uses such an interrupt to flash the cursor and scan the keyboard, so we better
                   ;stop it.

        lda $dc0d  ;by reading this two registers we negate any pending CIA irqs.
        lda $dd0d  ;if we don't do this, a pending CIA irq might occur after we finish setting up our irq.
                   ;we don't want that to happen.

        lda #$01   ;this is how to tell the VICII to generate a raster interrupt
        sta $d01a
        sta $2          ; flag to indicate state

        lda #48   ;this is how to tell at which rasterline we want the irq to be triggered
        sta $d012

        lda #$1b   ;as there are more than 256 rasterlines, the topmost bit of $d011 serves as
        sta $d011  ;the 9th bit for the rasterline we want our irq to be triggered.
                  ;here we simply set up a character screen, leaving the topmost bit 0.

        lda #$35   ;we turn off the BASIC and KERNAL rom here
        sta $01    ;the cpu now sees RAM everywhere except at $d000-$e000, where still the registers of
                   ;SID/VICII/etc are visible

        lda #<irq  ;this is how we set up
        sta $fffe  ;the address of our interrupt code
        lda #>irq
        sta $ffff


; Sprite pointer sprite 5
        LDA #128
        STA $07FD
        LDA #128
        STA $07FC

        spriteMode 0       ; all sprites are hi res

        spriteColor 5, RED ; Set hi res sprite color
        spriteXpos 5, 230
        spriteYPos 5, 180  ; Y pos sprite 5 = 180
        spriteOn   5       ; enable sprite 5

        spriteColor 4, YELLOW; Set hi res sprite color
        spriteXpos 4, 130
        spriteYPos 4, 180  ; Y pos sprite 5 = 180
        spriteOn   4       ; enable sprite 5

        cli        ;enable maskable interrupts again

        jmp *      ;we better don't RTS, the ROMS are now switched off, there's no way back to the system

irq
        lda     2
        beq     x1      ; flag == zero?

        setBorderColor CYAN

        lda     #0
        sta     2

        lda #252   ;this is how to tell at which rasterline we want the irq to be triggered
        sta $d012

        inc     $0403

        lda #$ff   ;this is the orthodox and safe way of clearing the interrupt condition of the VICII.
        sta $d019  ;if you don't do this the interrupt condition will be present all the time and you end
           ;up having the CPU running the interrupt code all the time, as when it exists the
           ;interrupt, the interrupt request from the VICII will be there again regardless of the
           ;rasterline counter.

        rti

x1
        setBorderColor BLACK
;        lda     #14
;        sta     53280

        lda     #1
        sta     2       ; flag = 1

        lda #48   ;this is how to tell at which rasterline we want the irq to be triggered
        sta $d012


        inc     $0400

        lda #$ff   ;this is the orthodox and safe way of clearing the interrupt condition of the VICII.
        sta $d019  ;if you don't do this the interrupt condition will be present all the time and you end

        rti

        *=$2000
sprite
 BYTE 0,124,0
 BYTE 7,207,192
 BYTE 12,0,112
 BYTE 24,0,24
 BYTE 48,0,12
 BYTE 33,129,132
 BYTE 97,129,132
 BYTE 64,8,4
 BYTE 192,8,6
 BYTE 128,8,2
 BYTE 128,8,2
 BYTE 128,8,2
 BYTE 192,8,2
 BYTE 64,128,194
 BYTE 96,128,134
 BYTE 32,193,132
 BYTE 48,127,12
 BYTE 24,12,24
 BYTE 12,0,96
 BYTE 7,128,192
 BYTE 0,255,0

