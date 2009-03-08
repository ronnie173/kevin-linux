#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

static int cmpstringp(const void *p1, const void *p2) {
    /* The actual arguments to this function are "pointers to
     pointers to char", but strcmp(3) arguments are "pointers
     to char", hence the following cast plus dereference */

    return strcmp(* (char * const *) p1, * (char * const *) p2);
}

int main(int argc, char *argv[]) {
    int j;
    qsort(&argv[1], argc - 1, sizeof(argv[1]), cmpstringp);

    for (j = 1; j < argc; j++)
        puts(argv[j]);
  
    exit(EXIT_SUCCESS);
}
