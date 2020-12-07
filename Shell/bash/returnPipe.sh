#!/bin/bash

returnSimple() {
    local  __resultvar=$1
    echo "Sub shell level: $BASH_SUBSHELL"
    return 42
}

echo "Sub shell level: $BASH_SUBSHELL"
returnSimple theResult | cat
result=${PIPESTATUS[0]}
echo ${result}
echo Done.
