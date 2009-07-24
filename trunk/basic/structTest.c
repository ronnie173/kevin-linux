/*
*/
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>

typedef struct _name_t {
    char lastName[25];
    char firstName[25];
} name_t;

int nameStructTest();
int convertTest();
int dumpName(const name_t *tmp);
 
int main(int argc, char *argv[]) {
    nameStructTest();
    convertTest();
    
    return 0;
}

int nameStructTest() {
    name_t two, three, four;

    name_t one = { .lastName = "Zhang", .firstName = "Kevin" };
    //strcpy(one.lastName, "Zhang");
    //strcpy(one.firstName, "Kevin");
    dumpName(&one);

    two = one;
    dumpName(&two);

    strcpy(two.lastName, "Yin");
    dumpName(&two);

    three = (name_t) { "Yin", "Nawei" };
    dumpName(&three);
    
    four = (name_t) { lastName: "Zhang", firstName: "Ryan" };
    dumpName(&four);

    return 0;
}

int convertTest() {
    char *p = NULL;
    name_t *tp = NULL;
    
    p = malloc(100);
    bzero(p, 100);
    tp = (name_t*)p;
    
    strcpy(tp->lastName, "Guo");
    strcpy(tp->firstName, "Zhang");
    
    dumpName(tp);
    
    return 0;
}

int dumpName(const name_t *tmp) {
    printf("in file %s, at line %d\n", __FILE__, __LINE__);
    if (NULL == temp) return 0;
    
    printf("first: [%s], last: [%s]\n", tmp->firstName, tmp->lastName);
    return 0;
}

