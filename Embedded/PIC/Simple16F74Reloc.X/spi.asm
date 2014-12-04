            PROCESSOR 16f74
            INCLUDE p16f74.inc

#define __SPI_ASM__
            INCLUDE spi.inc

            code

            global  SPIInit
SPIInit:
            BSF             USB_CS                           ; set C2 (/CS for USB interface)
            BSF             PORTC, 1                         ; set C1 (/CS for port expander)

            banksel TRISC                            ; bank 1
            MOVLW   B'11010001'                      ; C1/C2 output (CS), C3 output (SCK), C4 input (SDI), C5 output (SDO),
            MOVWF   TRISC

            MOVLW   B'00000000'                      ; !!!! FOR USBN9604, SSPSTAT.CKE must be 0!!!!!! (Data transmitted on falling edge)
    ;       MOVLW   B'11000000'                      ; !!!! FOR MCP23S17, SSPSTAT.CKE must be 1!!!!!! (Data transmitted on rising edge)
                                                                     ; !!!! Also, SSPSTAT.SMP is relevant - data input sampled at end of data output time
                                                                     ; otherwise, data is not properly read back!
            MOVWF   SSPSTAT

            banksel SSPCON
    ; for MCP23S17, both Idle low and idle high are working! Edge (and sample time) is relevant, though!
            MOVLW   B'00100010'                     ; SSPEN=1, CK Idle=Low, SPI master mode, clock = FOSC/64
    ;       MOVLW   B'00110010'                     ; SSPEN=1, CK Idle=High, SPI master mode, clock = FOSC/64
            MOVWF   SSPCON

            return

            end
