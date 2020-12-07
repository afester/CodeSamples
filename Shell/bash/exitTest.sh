#!/bin/bash

exitInSecondFunc() {
   echo "In exitInSecondFunc()"
   exit
   echo "Still in exitInSecondFunc()"
}

exitInFunc() {
   echo "In exitInFunc()"
#   exit
   exitInSecondFunc
   echo "Still in exitInFunc()"
}


#echo "Simple exit"
#exit
#echo "still here"

echo "Testing exit in func..."
exitInFunc
echo "Script still running ..."
