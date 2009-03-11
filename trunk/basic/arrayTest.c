/*
*/
#include <stdio.h>

#define MAX 10
int main(int argc, char *argv[]) {
    int i, a[MAX], j;
    
    if (&i == &a[MAX]) printf("a[MAX] equals i\n");
    
    if (&j == &a[MAX]) printf("a[MAX] equals j\n");
    
    return 0;
}
