            PROCESSOR   16f74
            INCLUDE     p16f74.inc

            INCLUDE     table.inc
            INCLUDE     instructions.inc
            INCLUDE     serial.inc

            udata_shr                   ; NOTE: This should be udata - verify the map file!
            global   Reg1, Reg2
Reg1        res 1
Reg2        res 1
Reg3        res 1


            code
            global  Main
            extern  hexToDigits
Main:
            call    SerialInit

            TBLINIT startMsg
            call    SerialSendConstString

            clrf    Reg3

CountLoop:
            MOVFF   Reg3, Reg1
            call    SerialSendBin       ; send value in Reg1 as binary digits
;            call    SerialSendHex       ; send value in Reg1 as hex digits

            movlw   '\n'
            call    SerialSendChar

            incf    Reg3, F
            movlw   0x0f
            xorwf   Reg3, W
            bnz     CountLoop

            TBLINIT doneMsg
            call    SerialSendConstString

Endless:
            goto    Endless

startMsg:   de      "\nRunning...\n\0"
doneMsg:    de      "Done.\0"

            end
