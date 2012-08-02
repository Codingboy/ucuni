#! /bin/bash

mkdir bin
gcc -o bin/test src/test.c -lusb-1.0 -I/usr/include/libusb-1.0
sudo bin/test
