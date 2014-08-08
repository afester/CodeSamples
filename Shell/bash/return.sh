#!/bin/bash

makeName() {
    local  __type=$1
    local  __fileName=$2
    local ext

#    local  __resultvar=$3
    declare -n myresult=$3

    echo -n "Enter Ext: "
    read ext
    myresult="$__fileName.$__type.$ext.log"

    # bash < 3.1
    # eval $__resultvar="'$myresult'"

    # bash >= 3.1
    # printf -v "$__resultvar" '%s' "$myresult"

    # bash >= 4.3
}

echo -n "Enter fileName:"
read fileName

makeName "type1" $fileName theResult ; name1=${theResult}
makeName "type2" $fileName theResult ; name2=${theResult}

echo $name1
echo $name2
