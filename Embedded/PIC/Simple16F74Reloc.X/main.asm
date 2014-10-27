        PROCESSOR 16f74
        INCLUDE   p16f74.inc


        udata_shr                   ; NOTE: This should be udata - verify the map file!
        global   Reg1, Reg2
Reg1    res 1
Reg2    res 1


        code
        global  Main
        extern  SerialInit, hexToDigits
Main:
        clrf    PORTA

        clrf    Reg1

        banksel ADCON1               ; bank 1 - ADCON1 and TRISA registers
        movlw   0x06                 ; all PORTA pins are
        movwf   ADCON1               ; digital
        movlw   B'00111100'          ; RA0, RA1 are outputs
        movwf   TRISA

        banksel PORTA               ; bank 0 - PORTA register

        call    SerialInit


        movlw   0x08
        movwf   Reg2
        call    hexToDigits

Loop:
        bsf     PORTA, 1
        bcf     PORTA, 1
        goto    Loop

        END
