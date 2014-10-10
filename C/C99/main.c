/**
 */

#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>
#include <complex.h>

typedef struct _complexType {
    float real;
    float imag;
} complexType;


complexType add(complexType a, complexType b) {
    complexType result = {a.real + b.real, a.imag + b.imag};
    return result;
}


int main() {

    // support for one-line comments beginning with //

// intermingled declarations and code
    printf("Hello World\n");
    int x = 0;
    printf("x=%d\n", x);

// designated initializers

    // Omitted field members are implicitly initialized the same as objects that have static storage duration.

    // arrays
    int values[10] = {[2] = 5, [7] = 32, [1] = 55};
    for (int i = 0;  i < 10;  i++) {
        printf(" %d. %d\n", i, values [i]);
    }
    int values2[10] = {1};
    for (int i = 0;  i < 10;  i++) {
        printf(" %d. %d\n", i, values2[i]);
    }

    // structures
    complexType ct = {.imag = 3, .real = 5};
    printf("Complex: %f + %fi\n", ct.real, ct.imag);

// compound literals - (T) {initializer-list}

    // especially useful when passing structures as parameters
    complexType res = add( (complexType){2, 3}, (complexType) {4, 5});
    printf("Result: %f + %fi\n", res.real, res.imag);

// New data types, including long long int, optional extended integer types,
// an explicit boolean data type, and a complex type to represent complex numbers
// new headers, such as <stdbool.h>, <complex.h>, <tgmath.h> (Type generic macros), and <inttypes.h>
    bool bVal = true;
    printf("Value: %s\n", bVal ? "true" : "false");

    double complex z1 = 2.0 + 3.0 * I;
    double complex z2 = 4.0 - 5.0 * I;

    printf("z1: %f + %fi\n", creal(z1), cimag(z1));
    printf("z2: %f + %fi\n", creal(z2), cimag(z2));
    printf("z2 + z2: %f + %fi\n", creal(z1+z2), cimag(z1+z2));

// improved support for IEEE floating point


    return 0;
}
