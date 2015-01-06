#!/bin/bash
gcc -o dseq dseq.c -L/usr/local/lib -llo

install dseq /usr/local/bin

rm dseq
