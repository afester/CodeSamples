#!/bin/bash

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
