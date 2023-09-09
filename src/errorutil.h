#include <stdio.h>

#include "../include/errorutil.h"

void print_error(Error err) {
    printf("ERROR: ");
    
    switch(err.type) {
        case ERROR_NONE:
            break;

        case ERROR_ARGUMENT:
            printf("Invalid argument");
            break;

        case ERROR_SYNTAX:
            printf("Invalid syntax");
            break;
    }
   
    putchar('\n');
    printf("     :%s\n", err.msg);
}
