#!/bin/bash

gcc -lncurses ioTest.c ../include/io.h ../src/io.c -lncurses -o ioTest
