#include "../h/workers.hpp"

uint64 fibonacci(uint64 n)
{
    if (n == 0 || n == 1) { return n; }
    if (n % 4 == 0) {
        thread_dispatch();
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void workerBodyA(void* arg)
{
    uint32 i = 0;
    for (; i < 3; i++)
    {
        printf("A:");
        uint64 temp = fibonacci(i);
        printf("%d", &temp);
    }

    printf("A yield");
    __putc('\n'); 
     __asm__ ("li t1, 7");
    thread_dispatch();

    uint64 t1 = 0;
    __asm__ ("mv %[t1], t1" : [t1] "=r"(t1));

    printf("A t1:");
    printf("%d", &t1);
    __putc('\n');

    uint64 result = fibonacci(20);
    printf("A fibonacci: ");
    printf("%lu", &result);
    __putc('\n');
    for (; i < 6; i++)
    {
        printf("A: i=");
        printf("%d",&i);
        printf("\n");
    }

    thread_dispatch();
}

void workerBodyB(void* arg)
{
    uint32 i = 10;
    for (; i < 13; i++)
    {
        printf("B: i=");
        printf("%d:",&i);
        printf("\n");
    }

    printf("B: yield");
    printf("\n");
    __asm__ ("li t1, 5");
    thread_dispatch();


    uint64 result = fibonacci(23);
    printf("B: fibonaci=");
    printf("%lu",&result);
    __putc('\n');

    for (; i < 16; i++)
    {
        printf("B: i=");
        printf("%d",&i);
        __putc('\n');
    }

    thread_dispatch();
}
