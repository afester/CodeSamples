
            PROCESSOR 16f74
            INCLUDE p16f74.inc

#define __MCP23x17_ASM__
            INCLUDE mcp23x17.inc

            code

            extern  Reg1, Reg2

;
;
            global  MCP23x17Init
MCP23x17Init:
            movlw   0x00                        ; all pins are output
            movwf   Reg1
            WriteMCP23S17   MCP23x17b0_IODIRA

            movlw   0x00                        ; all pins are output
            movwf   Reg1
            WriteMCP23S17   MCP23x17b0_IODIRB

            return

            end
