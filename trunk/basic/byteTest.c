/*
*/
#include <stdio.h>

#define MAX 10
int main(int argc, char *argv[]) {
    unsigned char space = ' ';
    int ctr = 0;
    
    printf("ASCII char ========\n");
    
    for (ctr = 0; ctr + space < 256; ctr++) {
        printf("%c", ctr + space);
    }
    
    printf("\n");
    
    return 0;
}
