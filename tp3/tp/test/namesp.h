#ifndef __NAMESP_H__
#define __NAMESP_H__

#include <stdio.h>

struct  TestNamespace {

    int (*add)(int, int);
    void (*printInt) (int);
    
};

int __test__add(int, int);
void __test__printInt(int);

#endif
