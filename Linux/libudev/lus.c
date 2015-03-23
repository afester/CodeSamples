#include <stdlib.h>
#include <stdio.h>

#include "usblookup.h"

int main() {
    char* vendor = NULL;
    char* product = NULL;

    getModelById(0x03eb, 0x0902, &vendor, &product);
    printf("RESULT: %s by %s\n", product, vendor);
    free(vendor);
    free(product);

    getModelById(0x03f0, 0x0117, &vendor, &product);
    printf("RESULT: %s by %s\n", product, vendor);
    free(vendor);
    free(product);

    getVendorById(0x03f0, &vendor);
    printf("RESULT: %s\n", vendor);
    free(vendor);
}
