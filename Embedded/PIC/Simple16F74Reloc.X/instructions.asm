
            PROCESSOR   16f74
            INCLUDE     p16f74.inc

#define __INSTRUCTIONS_ASM__
            INCLUDE instructions.inc

            udata_shr                   ; NOTE: This should be udata - verify the map file!
            global   mWsav
mWsav       res 1

            end