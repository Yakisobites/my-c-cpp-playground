#include <stdio.h>

#include "add.h"

int main(void)
{
    int result = add(2, 3);
    printf("Hello from CMake C project. 2 + 3 = %d\n", result);
    return 0;
}
