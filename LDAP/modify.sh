#!/bin/bash

ldapmodify -Y EXTERNAL -H ldapi:/// -f setConfigPass.ldif
