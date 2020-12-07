#!/bin/bash

returnSimple() {
    local  __resultvar=$1
    echo "Sub shell level: $BASH_SUBSHELL"
    printf -v "$__resultvar" '%s' "ERROR"
}

echo "Sub shell level: $BASH_SUBSHELL"
returnSimple theResult > /dev/null
echo ${theResult}
echo Done.
