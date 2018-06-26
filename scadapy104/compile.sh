#!/bin/sh
################# for WINDOWS ###############
gcc -g   -g -o ./bin/iec104server.exe iec104server.c ./parson/parson.c -I../src/inc/api -I../src/hal/inc -I../src/tls -I./parson ../build/lib60870.a -lpthread
