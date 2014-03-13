#include <stdio.h>
#include <stdlib.h>

#include "../../Tools/Profile/Profile.h"

int main() {
    const int size = 100000;
    int array[size];
    //set some items to 0 and other items to 1
    // I'd like to replace all items that have value of 1 with another value, for example 123456. This can be trivially implemented with:

    ProcessTimeCounter pt;
    ProcessTimeCounter_start(&pt);

    // we need to loop several times in order to measure anything at all
    for (int count = 0;  count < 5000;  count++) {
       for(int i = 0; i < size ; i++){
           if(array[i] != 0) {
               array[i] = 123456;
          }
       }
    }

    ProcessTimeCounter_stop(&pt);
    const char* result = ProcessTimeCounter_toString(&pt);
    printf("%s\n", result);
    free(result);
}
