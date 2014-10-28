;
;



; Define a read only table. Together with the "td" macros below, this can be used
; as follows to define a lookup table:
;
; TableName:    table
;               td  1
;               td  2
;               td2 4, 8
table       macro
; "When doing a table read using a computed GOTO method, care
;  should be exercised if the table location crosses a PCL
;  memory boundary (each 256 byte block). Refer to the
;  Application Note, “Implementing a Table Read" (AN556)"
; Well, errrr, yes...
;
            addwf   PCL, F
            endm


; Lookup a value from a read only table in ROM
;
; @param tableName  The name of the table from where to look up the value
; @param Reg1	    The index inside the table to retrieve
; @return W         The value at the given index
xlut        macro   tableName
			movlw 	HIGH tableName
			movwf 	PCLATH
			movf	Reg1, W
			call	tableName
            endm


; Initialize the table pointer with a code memory address
;
; @param addr The address of the ROM table
TBLINIT     macro   addr
            banksel PMADRH          ; bank2

            MOVLW   HIGH addr
            MOVWF   PMADRH
            MOVLW   LOW addr
            MOVWF   PMADR

            endm


; Read one byte from code memory.
;
; @return the current byte in W
;
; NOTE: After this macro has been executed, bank2 is selected!
;       On 16F74, bank2 GPRs select bank0 GPRs!
TBLRD       macro
            banksel PMCON1          ; bank3
            BSF     PMCON1, RD
            NOP
            NOP

            banksel PMDATA          ; bank2
            MOVF    PMDATA, W
            endm

; Increments the table pointer to the next address
;
; NOTE: After this macro has been executed, bank2 is selected!
;       On 16F74, bank2 GPRs select bank0 GPRs!
TBLINC      macro
            local   NoOvl

            banksel PMADRH          ; bank2
            INCF    PMADR, F
            BNZ     NoOvl
            INCF    PMADRH, F
NoOvl:
            endm


; Adds a specific index to the current table pointer
;
; NOTE: After this macro has been executed, bank2 is selected!
;       On 16F74, bank2 GPRs select bank0 GPRs!
TBLIDX      macro   offset
            local   NoOvl

            banksel PMADRH          ; bank2
            MOVLW   offset
            ADDWF   PMADR, F
            BNC     NoOvl
            INCF    PMADRH, F
NoOvl:
            endm


; Adds W as an index to the current table pointer
;
; @param W Contains the offset to add to the current table pointer
;
; NOTE: After this macro has been executed, bank2 is selected!
;       On 16F74, bank2 GPRs select bank0 GPRs!
TBLIDXW     macro
            local   NoOvl

            banksel PMADRH          ; bank2
            ADDWF   PMADR, F
            BNC     NoOvl
            INCF    PMADRH, F
NoOvl:
            endm
