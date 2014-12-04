/**
 */

#include <stdio.h>
#include <stdlib.h>

void universalCharacterName() {
    // universal character names, which allows user variables to contain other characters than the standard character set
    // need to be explicitly enabled with -fextended-identifiers
    // See also https://www.securecoding.cert.org/confluence/display/seccode/PRE30-C.+Do+not+create+a+universal+character+name+through+concatenation
    // "In general, avoid universal character names in identifiers unless absolutely necessary."

    int \u4F60\u597D = 42;
    printf("value=%d\n", \u4F60\u597D);

    const char* text = "Hello \u4F60\u597D";
    printf("%s\n", text);
}
