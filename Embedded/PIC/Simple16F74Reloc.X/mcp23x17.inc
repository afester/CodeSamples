;
;

#ifndef __MCP23x17_INC__
#define __MCP23x17_INC__


#ifndef __MCP23x17_ASM__

        extern  MCP23x17Init

#endif

                include spi.inc

; Bank0 configuration (POR configuration)
MCP23x17b0_IODIRA   EQU         H'00'   ; Port A direction
MCP23x17b0_IODIRB   EQU         H'01'   ; Port B direction
MCP23x17b0_IPOLA    EQU         H'02'
MCP23x17b0_IPOLB    EQU         H'03'
MCP23x17b0_GPINTENA EQU         H'04'
MCP23x17b0_GPINTENB EQU         H'05'
MCP23x17b0_DEFVALA  EQU         H'06'
MCP23x17b0_DEFVALB  EQU         H'07'
MCP23x17b0_INTCONA  EQU         H'08'
MCP23x17b0_INTCONB  EQU         H'09'
MCP23x17b0_IOCON    EQU         H'0A'   ; IOCON
; MCP23x17b0_IOCON    EQU               H'0B'   ; IOCON
MCP23x17b0_GPPUA    EQU         H'0C'
MCP23x17b0_GPPUB    EQU         H'0D'
MCP23x17b0_INTFA    EQU         H'0E'
MCP23x17b0_INTFB    EQU         H'0F'
MCP23x17b0_INTCAPA  EQU         H'10'
MCP23x17b0_INTCAPB  EQU         H'11'
MCP23x17b0_GPIOA    EQU         H'12'   ; Port A
MCP23x17b0_GPIOB    EQU         H'13'   ; Port B
MCP23x17b0_OLATA    EQU         H'14'   ; Port A latch
MCP23x17b0_OLATB    EQU         H'15'   ; Port B latch

; Bank1 configuration
MCP23x17b1_IODIRA   EQU         H'00'   ; Port A direction
MCP23x17b1_IPOLA    EQU         H'01'
MCP23x17b1_GPINTENA EQU         H'02'
MCP23x17b1_DEFVALA  EQU         H'03'
MCP23x17b1_INTCONA  EQU         H'04'
MCP23x17b1_IOCON    EQU         H'05'   ; IOCON
MCP23x17b1_GPPUA    EQU         H'06'
MCP23x17b1_INTFA    EQU         H'07'
MCP23x17b1_INTCAPA  EQU         H'08'
MCP23x17b1_GPIOA    EQU         H'09'   ; Port A
MCP23x17b1_OLATA    EQU         H'0A'   ; Port A latch

MCP23x17b1_IODIRB   EQU         H'10'   ; Port B direction
MCP23x17b1_IPOLB    EQU         H'11'
MCP23x17b1_GPINTENB EQU         H'12'
MCP23x17b1_DEFVALB  EQU         H'13'
MCP23x17b1_INTCONB  EQU         H'14'
; MCP23x17b1_IOCON    EQU               H'15'   ; IOCON
MCP23x17b1_GPPUB    EQU         H'16'
MCP23x17b1_INTFB    EQU         H'17'
MCP23x17b1_INTCAPB  EQU         H'18'
MCP23x17b1_GPIOB    EQU         H'19'   ; Port B
MCP23x17b1_OLATB    EQU         H'1A'   ; Port B latch


; Writes a value into the port expander
;
; @param Reg1 The value to write
; @param addr The address of the register
WriteMCP23S17   macro   addr

                BCF     SSPCON, SSPEN       ; disable SSP
                banksel SSPSTAT
                BSF     SSPSTAT, CKE        ; reconfigure CKE
                banksel PORTC               ; bank 0
                BSF     SSPCON, SSPEN       ; enable SSP

                BCF     PORTC, 1            ; select device (RC1)

                movlw   b'01000000'         ; Write Opcode
                WriteSPI

                movlw   addr                ; address
                WriteSPI

                movf    Reg1, W             ; value
                WriteSPI

                BSF     PORTC, 1            ; deselect device (RC1)

                BCF     SSPCON, SSPEN       ; disable SSP
                banksel SSPSTAT
                BCF     SSPSTAT, CKE        ; reconfigure CKE
                banksel PORTC               ; bank 0
                BSF     SSPCON, SSPEN       ; enable SSP

                endm

#endif
