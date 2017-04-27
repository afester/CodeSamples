#!/bin/bash

set -x

someCommand() {
   echo "command ARGC: $#"
   echo $*
}

execEcho() {
   echo "ARGC: $#"
   someCommand "$@"
}

VALUE="Test 123"
execEcho Hello $VALUE
execEcho "Hello World $VALUE"
