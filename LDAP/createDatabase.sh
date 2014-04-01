#!/bin/bash

ldapadd -Y EXTERNAL -H ldapi:/// -f database.ldif
