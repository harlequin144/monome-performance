#!/bin/bash
gcc -o dtrig dtrig.c -L/usr/local/lib -llo

install dtrig /usr/local/bin

rm dtrig
