#!/bin/bash

gcc -lncurses ioTest.c ../include/common.h ../include/io.h ../src/io.c -lncurses -o ioTest
