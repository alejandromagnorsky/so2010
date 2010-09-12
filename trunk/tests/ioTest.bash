#!/bin/bash

gcc -lncurses -lm ioTest.c ../include/common.h ../include/io.h ../src/io.c -lncurses -o ioTest
