        PROCESSOR 16f74
        INCLUDE   p16f74.inc
        __CONFIG  _CP_OFF & _WDT_OFF & _HS_OSC

; ********************************************************
; Reset Vector
; ********************************************************
        extern  Main
Reset   code    0x00                ; reset Vector at fixed address

        bcf     STATUS, RP1         ; Select
        bcf     STATUS, RP0         ; Bank 0
        goto    Main


; ********************************************************
; Interrupt service routines
; ********************************************************
IntVect code    0x04                ; Interrupt Vector at fixed address
IntReq:
        retfie

        END
