#!/bin/bash

wget https://github.com/antlr/website-antlr4/raw/gh-pages/download/antlr-4.1-complete.jar

wget http://download.forge.ow2.org/asm/asm-4.1-bin.zip
unzip -j asm-4.1-bin.zip asm-4.1/lib/all/asm-all-4.1.jar 
rm asm-4.1-bin.zip
