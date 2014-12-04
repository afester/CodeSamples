/*
 * array.c
 *
 *  Created on: 09.08.2013
 *      Author: afester
 */

#if 0

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const int MAX_ITEMS = 200;
const int MAX_ITEM_LEN = 100;

int main2() {
    int i = 0;
    int no_of_item = 0;
    char* tempstr = NULL;

    char *menu_list_down[MAX_ITEMS]; /* array with 200 pointers (uninitialized) */
    for (i = 0; i < MAX_ITEMS; i++) {      /* initialize array elements */
        menu_list_down[i] = NULL;
    }

    tempstr = strdup("Alu Tomato");
    menu_list_down[no_of_item] = tempstr;
    no_of_item++;

    tempstr = strdup("Baigan Bharta");
    menu_list_down[no_of_item] = tempstr;
    no_of_item++;

    tempstr = strdup("Chicken 65");
    menu_list_down[no_of_item] = tempstr;
    no_of_item++;

    for (i = 0; i < no_of_item; i++) {
        printf("%s\n", menu_list_down[i]);
    }

    return 0;
}
#endif


#include<stdio.h>
/* #define hello (2813 / 30.0)
 */
/*
const float hello = 2813 / 30.0;

int main(){
    float x,y;
    printf("enter:");
    scanf("%f",&x);
    y = hello * x;
    printf("%.2f",y);

    return 0;
}
*/
