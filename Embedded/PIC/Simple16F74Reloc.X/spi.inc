;
;

#ifndef __SPI_INC__
#define __SPI_INC__

#ifndef __SPI_ASM__

        extern  SPIInit

#endif



        include instructions.inc

#define USB_CS PORTC, 2


; Performs a single SPI transaction
; one byte is written, and at the same time one byte is read
;
; @param W-reg contains the value to write
; @return W-reg contains the value which has been read back
WriteSPI        macro
                        local   LOOP

                        MOVWF   SSPBUF                          ; New data to xmit

                        banksel SSPSTAT                         ; bank 1
LOOP:           BTFSS   SSPSTAT, BF                     ; Has data been received (transmit complete)?
                        BRA     LOOP                            ; No

                        banksel SSPBUF                          ; bank 0
                        MOVF    SSPBUF, W               ; W reg = contents of SSPBUF

                        endm

#endif
