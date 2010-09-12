#!/bin/bash

gcc -lncurses -lm ioTest.c ../include/common.h ../include/io.h ../include/common.h ../src/io.c -lncurses -o ioTest
