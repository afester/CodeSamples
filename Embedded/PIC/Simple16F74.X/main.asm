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
        goto    Main

        END
