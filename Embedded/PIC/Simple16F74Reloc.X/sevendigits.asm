            PROCESSOR 16f74
            INCLUDE p16f74.inc
            INCLUDE mcp23x17.inc
            INCLUDE instructions.inc
            INCLUDE table.inc

#define __SEVENDIGITS_ASM__
            INCLUDE sevendigits.inc

            extern  Reg1, Reg2
            code

;
;
            global  ssegSetValue
ssegSetValue:
; Set low digit
            TBLINIT ssegGetDigitTable   ; initialize table pointer

            movf    Reg2, W             ; load value from parameter
            andlw   0x0f                ; low nibble
            TBLIDXW                                             ; add offset to table pointer

            TBLRD                                               ; read value from table

            movwf   Reg1
            WriteMCP23S17   MCP23x17b0_OLATA    ; write value into PORT A latch

; set high digit
            TBLINIT ssegGetDigitTable   ; initialize table pointer

            swapf   Reg2, W             ; load high nibble from Parameter
            andlw   0x0f
            TBLIDXW                                             ; add offset to table pointer

            TBLRD                                               ; read value from table

            movwf   Reg1
            WriteMCP23S17   MCP23x17b0_OLATB    ; write value into PORT B latch

            return


; Convert a 4 bit binary value into the corresponding bitmask for a seven
; segment display
;
; @param    Reg1    The value to convert (only the lower 4 bits are used)
; @return   W       The converted value.
        global  hexToDigits
hexToDigits:
        banksel PMADRH          ; bank2
        MOVLW   HIGH ssegGetDigitTable
        MOVWF   PMADRH
        MOVLW   LOW ssegGetDigitTable
        MOVWF   PMADR

        movf    Reg2, W             ; load value from parameter
        andlw   0x0f                ; low nibble

        banksel PMADRH          ; bank2
        ADDWF   PMADR, F
        BNC     NoOvl
        INCF    PMADRH, F
NoOvl:
        banksel PMCON1          ; bank3
        BSF     PMCON1, RD
        NOP
        NOP

        banksel PMDATA          ; bank2
        MOVF    PMDATA, W

        return


; Seven segment digit encoding table
;       a
;     ====
; f  || g ||  b
;     ====
; e  ||   ||  c
;     ====
;       d
ssegGetDigitTable:
;         pgfedcba
        de      b'00111111'      ; 0
        de      b'00000110'      ; 1
        de      b'01011011'      ; 2
        de      b'01001111'      ; 3
        de      b'01100110'      ; 4
        de      b'01101101'      ; 5
        de      b'01111101'      ; 6
        de      b'00000111'      ; 7
        de      b'01111111'      ; 8
        de      b'01101111'      ; 9
;       de      b'01011111'      ; a
        de      b'01110111'      ; A
        de      b'01111100'      ; b
;       de      b'01011000'      ; c
        de      b'00111001'      ; C
        de      b'01011110'      ; d
;       de      b'01111011'      ; e
        de      b'01111001'      ; E
        de      b'01110001'      ; f

        END
