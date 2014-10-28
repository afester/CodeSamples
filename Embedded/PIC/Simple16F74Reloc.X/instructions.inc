;
;

            extern  mWsav

; 18Fxx2 instruction: MOVFF fs, fd
;
; Move fs (source) to  fd (destination)
; Side effect: modifies STATUS.Z
MOVFF   macro   source, dest
        MOVWF   mWsav           ; save W
        MOVF    source, W       ; load source
        MOVWF   dest            ; store into destination
        MOVF    mWsav, W        ; restore W
        endm


; 18Fxx2 instruction: SETF f
;
; Set all bits in f to 1
SETF    macro   addr
        MOVWF   mWsav           ; save W
        MOVLW   b'11111111'
        MOVWF   addr
        MOVF    mWsav, W        ; restore W
        endm


; 18Fxx2 instruction: BTG f, b
;
; Bit Toggle f
; Side effect: modifies STATUS.Z
BTG     macro   addr, bit
        MOVWF   mWsav               ; save W
        MOVLW   b'00000001' << bit  ; load mask into W
        XORWF   addr, F             ; invert bit in register
        MOVF    mWsav, W            ; restore W
        endm

; 18Fxx2 instruction: BRA addr
;
; Branch unconditionally
BRA     macro   addr
        GOTO    addr
        endm
