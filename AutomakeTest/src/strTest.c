/* String test */
#include <stdio.h>
#include <string.h>

int main(void) {
    char* source_str = "THIS IS THE SOURCE STRING";
    char* cPtr;

    /* Use printf() to print the first 11 characters of source_str. */
    printf("First 11 characters: ¡®%11s¡¯\n", source_str);
    /* Use printf() to print only the
    last 13 characters of source_str. */
    printf("Last 13 characters: ¡®%13s¡¯\n", source_str + (strlen(source_str) - 13));
    
    cPtr = source_str;
    printf("before %c\n", *cPtr++);
    printf("after %c\n", *cPtr);
    printf("after %c\n", *++cPtr);
    
    return 0;
}
