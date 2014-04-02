#!/bin/bash

# delete complete DIT below and including dc=example,dc=com
ldapdelete -r "dc=example,dc=com" -D "cn=admin,dc=example,dc=com" -x -w ldapAdmin
