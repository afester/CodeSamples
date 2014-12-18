#include <stdio.h>

typedef struct _String {
    int length;
    char* value;
}String;


int main() {
    String s1 = {0, NULL};
    String s2 = {0, NULL};

    String_assign(s1, "Hello");
    String_assign(s2, "World");
    String_concat(s1, s2);

    printf("%d %p\n", s1.length, s1.value);
    printf("%d %p\n", s2.length, s2.value);
    return 0;
}

