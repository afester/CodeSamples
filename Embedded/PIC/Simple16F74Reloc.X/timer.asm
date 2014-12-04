
            PROCESSOR 16f74
            INCLUDE p16f74.inc

#define __TIMER_ASM__
            INCLUDE timer.inc

            code

            extern  Reg1, Reg2

;
;
            global  TimerInit
TimerInit:
 
;            MOVLW   .10
;            MOVWF   count

    ; Initialize Timer 1 to provide a 10 Hz timebase
            banksel TMR1L
            CLRF    TMR1L               ; Clear Low byte, Ensures no rollover i
            MOVLW   high TMR_VAL        ; Value to load into TMR1H
            MOVWF   TMR1H               ; Write High byte
            MOVLW   low TMR_VAL         ; Value to load into TMR1L
            MOVWF   TMR1L               ; Write Low byte

            MOVLW   B'00110000'         ; Timer stopped
                                        ; Internal clock (Fosc/4)
                                        ; Prescale 1:8 => 3,2µs / 312,5 kHz
            MOVWF   T1CON               ; initialize timer1 control word

            banksel PIE1
            MOVLW   B'00000001'
            MOVWF   PIE1                ; enable TMR1 interrupt

            MOVLW   B'11010000'         ; GIE=1, PEIE=1, INTE=1
    ;       MOVWF   INTCON              ; Enable interrupts

            banksel T1CON
            BSF     T1CON, TMR1ON       ; start timer1

            return

            end
