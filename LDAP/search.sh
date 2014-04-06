#!/bin/bash

ldapsearch -Y EXTERNAL -H ldapi:/// -b 'dc=example,dc=com' "cn=John Doe"

ldapsearch -x -D"cn=John Doe,ou=people,dc=example,dc=com" -w example -b 'dc=example,dc=com'  "uid=johndoe" userPassword
