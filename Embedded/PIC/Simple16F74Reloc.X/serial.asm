        PROCESSOR 16f74
        INCLUDE   p16f74.inc

        code
        global  SerialInit

SerialInit:
; Initialize Serial I/O
        BANKSEL      SPBRG           ; Bank 1

;       MOVLW   .7                   ; initialize baud rate generator (19200 bps)
        MOVLW   .10                  ; initialize baud rate generator (57600 bps)
        MOVWF   SPBRG

        MOVLW   B'00100000'          ; initialize serial port transmit con
        MOVWF   TXSTA
        BSF     TXSTA, BRGH

        BANKSEL RCSTA                ; Bank 0

        MOVLW   B'10010000'          ; initialize serial port receive cont
        MOVWF   RCSTA

        return

        end
