            PROCESSOR 16f74
            INCLUDE p16f74.inc
            INCLUDE table.inc

            code

            extern  Reg1, Reg2


; Initializes the serial UART interface.
;
            global  SerialInit
SerialInit:
; Initialize Serial I/O
            BANKSEL SPBRG               ; Bank 1

;            MOVLW   .7                     ; initialize baud rate generator (19200 bps)
            MOVLW   .10                 ; initialize baud rate generator (57600 bps)
            MOVWF   SPBRG

            MOVLW   B'00100000'         ; initialize serial port transmit con
            MOVWF   TXSTA
            BSF     TXSTA, BRGH

            BANKSEL RCSTA               ; Bank 0

            MOVLW   B'10010000'         ; initialize serial port receive cont
            MOVWF   RCSTA

            return


; Sends a single character to the serial interface.
;
; @param W the character to transmit.
            global  SerialSendChar
SerialSendChar:
; wait for transmit buffer ready
            BANKSEL TXSTA               ; Bank 1
loop:
            btfss   TXSTA, TRMT
            goto    loop

; send next character
            BANKSEL TXREG               ; Bank 0
            MOVWF   TXREG

            return


; Sends a constant, null terminated string from the ROM to the serial interface.
;
; @param PMADDRH/PMADDR The code memory address of the string to send.
            global  SerialSendConstString
SerialSendConstString:
            TBLRD
            btfsc   STATUS, Z
            return

            call    SerialSendChar

            TBLINC
            goto    SerialSendConstString


; Sends a two-digit hex value to the serial interface.
;
; @param Reg1   The value to send as two-digit hex string
;
            global  SerialSendHex
SerialSendHex:

; send high digit
            TBLINIT hexChars            ; initialize table pointer

            swapf   Reg1, W             ; load high nibble from Parameter
            andlw   0x0f
            TBLIDXW                         ; add offset to table pointer

            TBLRD                           ; read value from table
            call    SerialSendChar

; send low digit
            TBLINIT hexChars            ; initialize table pointer

            movf    Reg1, W                 ; load value from parameter
            andlw   0x0f                    ; low nibble
            TBLIDXW                         ; add offset to table pointer

            TBLRD                           ; read value from table
            call    SerialSendChar

            return


; Sends a 8 character binary string to the serial interface.
;
; @param Reg1   The value to send as binary string
;
; Uses Reg2 which will be 0 when the procedure returns.
;
            global  SerialSendBin
SerialSendBin:
            movlw   0x08
            movwf   Reg2

LoopSendBin:
            movlw   '0'
            rlf     Reg1, F
            btfsc   STATUS, C
            movlw   '1'
            call    SerialSendChar

            decfsz  Reg2, F
            goto    LoopSendBin

            return


hexChars:
            de      '0'
            de      '1'
            de      '2'
            de      '3'
            de      '4'
            de      '5'
            de      '6'
            de      '7'
            de      '8'
            de      '9'
            de      'a'
            de      'b'
            de      'c'
            de      'd'
            de      'e'
            de      'f'

            end
