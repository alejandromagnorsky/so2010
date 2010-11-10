#include "namesp.h"

extern struct TestNamespace Test;

int main() {
    
    int n = 10;
    
    n = Test.add(n, 5);
    Test.printInt(n);
   
    return 0; 
}
