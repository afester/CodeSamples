#!/bin/bash

# Creates a file name and returns it.
# $1 - the string to insert as file type
# $2 - the base filename
# $3 - the name of the variable which receives the result
makeName() {
    local  __type=$1
    local  __fileName=$2
    local ext

    echo -n "Enter Ext: "
    read ext

    # bash >= 4.3: declare myresult as Nameref
    # Then, simply assign to myresult
    # declare -n myresult=$3

    myresult="$__fileName.$__type.$ext.log"

    # bash < 3.1: use eval to set the variable
    # passed by name as parameter $3
    #local  __resultvar=$3
    #eval $__resultvar="'$myresult'"

    # bash >= 3.1: use print to set the variable
    # passed by name as parameter $3
    local  __resultvar=$3
    printf -v "$__resultvar" '%s' "$myresult"
}

echo -n "Enter fileName:"
read fileName

makeName "type1" $fileName theResult ; name1=${theResult}
makeName "type2" $fileName theResult ; name2=${theResult}

echo $name1
echo $name2
