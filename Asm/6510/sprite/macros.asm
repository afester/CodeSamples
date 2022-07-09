
; Note: Nested macros are not yet supported with CBM PRG Studio!


BLACK = 0
WHITE = 1
RED = 2
CYAN = 3
PURPLE = 4
GREEN = 5
BLUE = 6
YELLOW = 7
ORANGE = 8
BROWN = 9
PINK = 10
DARKGREY = 11
GREY = 12
LIGHTGREEN = 13
LIGHTBLUE = 14
LIGHTGREY = 15

defm    clrscr
        
        lda #$20
        ldx #0
loop
        sta $400,x
        sta $500,x
        sta $600,x
        sta $700,x      ; note: 24 bytes overflow! (Sprite pointer)
        inx
        bne loop

        endm

; Sets the background color
defm    setBgColor
        lda #/1
        sta 53281
        endm

; Sets the border color
defm    setBorderColor
        lda #/1
        sta 53280
        endm

; Sets the sprite mode mask - each set bit defines the sprite as multicolor
defm    spriteMode
        LDA #/1
        STA $D01C
        endm

; Sets the Y position of the given sprite
defm    spriteYPos

regSprYpos = 2 * /1
        LDA #/2
        STA $D001 + regSprYpos
        endm

; Sets the X position of the given sprite
defm    spriteXpos

sprXposSetMask = 1 << /1
sprXposClrMask = %11111111 XOR sprXposSetMask
regSprXpos = 2 * /1

        LDA #/2 AND $FF
        STA $D000 + regSprXpos

if /2 > 255
        lda #sprXposSetMask
        ora $0D10
else
        lda #sprXposClrMask
        and $0D10
endif
        sta $D010

        endm


; Sets the sprite color for hi res or for multi color pattern %10
defm    spriteColor
        LDA #/2
        STA $D027 + /1
        endm

; Enables sprite passed as parameter (0..7)
defm    spriteOn

mask = 1 << /1
        lda #mask
        ora $D015
        sta $D015

        endm

; disables sprite passed as parameter (0..7)
defm    spriteOff

mask2 = 1 << /1
mask3 = %11111111 XOR mask2
        lda #mask3
        and $D015
        sta $D015

        endm
