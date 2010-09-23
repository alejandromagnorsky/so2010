#!/bin/bash

gcc -lncurses -lm -g ioTest.c ../include/common.h ../include/io.h ../include/common.h ../src/io.c -lncurses -o ioTest
