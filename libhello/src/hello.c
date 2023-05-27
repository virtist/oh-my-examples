#include "hello.h"
#include <stdio.h>

int say_hello(void)
{
    printf("Hello world\n");
    return 0;
}

int say_hello2(void)
{
    return say_hello();
}