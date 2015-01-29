#!/bin/bash
#gcc `pkg-config --cflags gtk+-3.0` -o transport transport.c -L/usr/local/lib -llo -lrt `pkg-config --libs gtk+-3.0`

gcc -g -O0 `pkg-config --cflags gtk+-3.0` -o transport transport.c `pkg-config --libs gtk+-3.0` -L/usr/local/lib -llo -lrt 

#cc `pkg-config --cflags gtk+-3.0` -o transport transport.c `pkg-config --libs gtk+-3.0`-L/usr/local/lib -llo -lrt 



