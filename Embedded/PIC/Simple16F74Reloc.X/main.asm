            PROCESSOR   16f74
            INCLUDE     p16f74.inc
            INCLUDE     table.inc

            udata_shr                   ; NOTE: This should be udata - verify the map file!
            global   Reg1, Reg2
Reg1        res 1
Reg2        res 1


            code
            global  Main
            extern  SerialInit, SerialSendChar, SerialSendConstString, SerialSendHex, hexToDigits
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

            TBLINIT startMsg
            call    SerialSendConstString

            clrf    Reg1

CountLoop:
            call    SerialSendHex           ; send value in Reg1 as hex digits
            movlw   '\n'
            call    SerialSendChar

            incf    Reg1, F
            movlw   0x0f
            xorwf   Reg1, W
            bnz     CountLoop

            TBLINIT doneMsg
            call    SerialSendConstString

Endless:
            goto    Endless

startMsg:   de      "\nRunning...\n\0"
doneMsg:    de      "Done.\0"

            end
