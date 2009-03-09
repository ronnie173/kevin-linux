/*
*/
#include <stdio.h>
#include <time.h>
#include <string.h>

typedef struct _name_t {
    char lastName[25];
    char firstName[25];
} name_t;

int dumpName(const name_t *tmp);
 
int main(int argc, char *argv[]) {

    name_t one, two;

    strcpy(one.lastName, "Zhang");
    strcpy(one.firstName, "Kevin");
    dumpName(&one);

    two = one;
    dumpName(&two);

    strcpy(two.lastName, "Yin");
    dumpName(&two);

    return 0;
}

int dumpName(const name_t *tmp) {
    printf("in file %s, at line %d\n", __FILE__, __LINE__);
    printf("first: [%s], last: [%s]\n", tmp->firstName, tmp->lastName);
    return 0;
}

