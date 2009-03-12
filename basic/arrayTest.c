/*
*/
#include <stdio.h>

#define MAX 10
int main(int argc, char *argv[]) {
    int i = 2, a[MAX], j = 23;
    char *p = "hello world.";
    char *q = "hello world.";
    
    if (&i == &a[MAX]) {
        printf("a[MAX] equals i\n");
        printf("a[MAX] value is %d\n", a[MAX]);
    }
    
    if (&j == &a[MAX]) {
        printf("a[MAX] equals j\n");
        printf("a[MAX] value is %d\n", a[MAX]);
    }
    
    printf("p is [%s]\n", p);
    printf("q is [%s]\n", q);
    
    p[4] = '\0';
    
    printf("p is [%s]\n", p);
    printf("q is [%s]\n", q);

    return 0;
}
