        PROCESSOR 16f74
        INCLUDE   p16f74.inc

        code
        global  Main
Main:
        clrf    PORTA

        banksel ADCON1               ; bank 1 - ADCON1 and TRISA registers
        movlw   0x06                 ; all PORTA pins are
        movwf   ADCON1               ; digital
        movlw   B'00111100'          ; RA0, RA1 are outputs
        movwf   TRISA

        banksel PORTA               ; bank 0 - PORTA register

Loop:
        bsf     PORTA, 1
        bcf     PORTA, 1
        goto    Loop

        END
