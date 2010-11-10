/* 
** Struct-as-namespace demonstration
*/

#include "namesp.h"

struct TestNamespace Test = {  __test__add,
                        __test__printInt  };

int __test__add(int a, int b) {
    return a + b;
}

void __test__printInt(int a) {
    printf("Int %d\n", a);
}
