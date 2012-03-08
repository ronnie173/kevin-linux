/*
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10
int main(int argc, char *argv[]) {
    int i = 2, a[MAX], j = 23;
    char *p = "hello world.";
    //char p[] = "hello world.";
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
    
    //p[4] = '\0';

    p = (char *)malloc(20);
    strncpy(p, "hello world", 20);
    printf("p is [%s]\n", p);

/*    char *tmp = p + 4;
    printf("4 is %s\n", tmp);
    printf("4 char is [%c]\n", *tmp);
    *tmp = '\0';
*/
    p[4] = '\0';
    printf("p is [%s]\n", p);
    free(p);

    const char *strArray[3] = {
        [0] = "hello",
        [1] = "world",
        [2] = "kevin"
    };

    for (i = 0; i < 3; i++) {
        printf("index [%d], str [%s]\n", i, strArray[i]);
    }

    return 0;
}
