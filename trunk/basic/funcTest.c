#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void const_func(const int*);

float Plus(float a, float b);
float Minus(float a, float b);
float Multiply(float a, float b);
float Divide(float a, float b);
void Switch(float a, float b, char opCode);
void Switch_With_Function_Pointer(float a, float b, float(*pt2Func)(float,
        float));
void Replace_A_Switch();

int DoIt(float a, char b, char c);
int DoMore(float a, char b, char c);
void Array_Of_Function_Pointers();

int CmpFunc(const void* _a, const void* _b);
void QSortExample();

// type-definition: 'pt2Function' now can be used as type
typedef int (*pt2Function)(float, char, char);

int main(int argc, char *argv[]) {
    int x[10];
    int y;
    /* Set up the integer array. */
    for (y = 0; y < 10; y++) {
        x[y] = y;
    }

    /* Call const_func(), passing the x array by reference. */
    const_func(x);

    Replace_A_Switch();

    Array_Of_Function_Pointers();

    QSortExample();

    return 0;
}

int DoIt(float a, char b, char c) {
    printf("DoIt\n");
    return a + b + c;
}
int DoMore(float a, char b, char c) {
    printf("DoMore\n");
    return a - b + c;
}

//---------------------------------------------------------------------------
// 1.2 Introductory Example or How to Replace a Switch-Statement
// Task: Perform one of the four basic arithmetic operations specified by the
//       characters '+', '-', '*' or '/'.


// The four arithmetic operations ... one of these functions is selected
// at runtime with a swicth or a function pointer
float Plus(float a, float b) {
    return a + b;
}
float Minus(float a, float b) {
    return a - b;
}
float Multiply(float a, float b) {
    return a * b;
}
float Divide(float a, float b) {
    return a / b;
}

/*
 *  Solution with a switch-statement - <opCode> specifies which
 *  operation to execute
 */
void Switch(float a, float b, char opCode) {
    float result;

    // execute operation
    switch (opCode) {
    case '+':
        result = Plus(a, b);
        break;
    case '-':
        result = Minus(a, b);
        break;
    case '*':
        result = Multiply(a, b);
        break;
    case '/':
        result = Divide(a, b);
        break;
    }

    printf("Switch: 2+5=%f\n", result);
}

/*
 * Solution with a function pointer - <pt2Func> is a function pointer
 * and points to a function which takes two floats and returns a
 * float. The function pointer "specifies" which operation shall be executed.
 */
void Switch_With_Function_Pointer(float a, float b, float(*pt2Func)(float,
        float)) {
    float result = (*pt2Func)(a, b); // call using function pointer

    printf("Switch replaced by function pointer: 2-5=%f\n", result);
}

// Execute example code
void Replace_A_Switch() {
    printf("Executing function 'Replace_A_Switch'\n");

    /* '+' specifies function 'Plus' to be executed */
    Switch(2, 5, '+');
    /* pointer to function 'Minus' */
    Switch_With_Function_Pointer(2, 5, &Minus);
}

/* The const_function receives an integer array by reference.
Notice that the pointer is declared as const, which renders
it unmodifiable by the const_func() function. */
void const_func(const int* i) {
    int y;
    /* Print the contents of the integer array. */
    for (y = 0; y < 10; y++) {
        printf("%d\n", *(i+y));
        printf("%d\n", i[y]);
    }
}

// illustrate how to work with an array of function pointers
void Array_Of_Function_Pointers() {
    printf("\nExecuting 'Array_Of_Function_Pointers'\n");

    /* define arrays and ini each element to NULL, <funcArr1> and
       <funcArr2> are arrays with 10 pointers to functions which
       return an int and take a float and two char
    */
    // first way using the typedef
    pt2Function funcArr1[10] = { NULL };

    // 2nd way directly defining the array
    int (*funcArr2[10])(float, char, char) = {NULL};

    // assign the function's address - 'DoIt' and 'DoMore' are suitable functions
    // like defined above in 2.1-4
    funcArr1[0] = funcArr2[1] = &DoIt;
    funcArr1[1] = funcArr2[0] = &DoMore;

    /* more assignments */

    // calling a function using an index to address the function pointer
    printf("%d\n", funcArr1[1](12, 'a', 'b')); //  short form
    printf("%d\n", (*funcArr1[0])(12, 'a', 'b')); // "correct" way of calling
    printf("%d\n", (*funcArr2[1])(56, 'a', 'b'));
    printf("%d\n", (*funcArr2[0])(34, 'a', 'b'));
}

// comparison-function for the sort-algorithm
// two items are taken by void-pointer, converted and compared
int CmpFunc(const void* _a, const void* _b) {
    // you've got to explicitly cast to the correct type
    const float* a = (const float*) _a;
    const float* b = (const float*) _b;

    if (*a > *b)
        return 1; // first item is bigger than the second one -> return 1
    else if (*a == *b)
        return 0; // equality -> return 0
    else
        return -1; // second item is bigger than the first one -> return -1
}

// example for the use of qsort()
void QSortExample() {
    float field[100];

    srandom(99); // initialize random-number-generator
    int c;
    for (c = 0; c < 100; c++) // randomize all elements of the field
        field[c] = random();

    // sort using qsort()
    qsort((void*) field, /*number of items*/100, /*size of an item*/
            sizeof(field[0]),
            /*comparison-function*/&CmpFunc);

    // display first ten elements of the sorted field
    printf("The first ten elements of the sorted field are ...\n");
    for (c = 0; c < 100; c++)
        printf("element #%d contains %.0f\n", c + 1, field[c]);
    printf("\n");
}
