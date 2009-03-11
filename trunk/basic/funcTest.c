#include <stdio.h>

void const_func(const int*);

int main(void) {
    int x[10];
    int y;
    /* Set up the integer array. */
    for (y=0; y<10; y++)
        x[y] = y;
    /* Call const_func(), passing the x array by reference. */
    const_func(x);

    return 1;
}

/* The const_function receives an integer array by reference.
Notice that the pointer is declared as const, which renders
it unmodifiable by the const_func() function. */
void const_func(const int* i) {
    int y;
    /* Print the contents of the integer array. */
    for (y=0; y<10; y++) {
        printf("%d\n", *(i+y));
        printf("%d\n", i[y]);
    }
}
