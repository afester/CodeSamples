        PROCESSOR 16f74
        INCLUDE   p16f74.inc
        __CONFIG  _CP_OFF & _WDT_OFF & _HS_OSC

;        udata
      udata_shr     ; note that this should be udata - verify the map file!
Counter1:   res 1

        code
        nop

        end
