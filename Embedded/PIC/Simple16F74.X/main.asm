        PROCESSOR 16f74
        INCLUDE   p16f74.inc
        __CONFIG  _CP_OFF & _WDT_OFF & _HS_OSC

Reset:
        ORG     0x0

        bcf     STATUS, RP1          ; Select
        bcf     STATUS, RP0          ; Bank 0
        goto    Main

; ********************************************************
; Interrupt service routine
; ********************************************************
        ORG     0x4

        retfie

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
