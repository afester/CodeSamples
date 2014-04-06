#!/bin/bash

ldapadd -f domain.ldif -D "cn=admin,dc=example,dc=com" -x -w ldapAdmin
ldapadd -f people.ldif -D "cn=admin,dc=example,dc=com" -x -w ldapAdmin
ldapadd -f persons.ldif -D "cn=admin,dc=example,dc=com" -x -w ldapAdmin
