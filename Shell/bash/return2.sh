#!/bin/bash

# NOTE: declare -n was introduced with Bash 4.3's Namerefs.
# http://mywiki.wooledge.org/BashFAQ/006

someFunction() {
    declare -n __resultVar=$1
    local someValue="Hello World"

    # The next statement assigns the value of the local variable to the variable
    # with the given name
    __resultVar=${someValue}
}

# call the function and pass the name of the result variable
someFunction theResult
echo ${theResult}
