#include <stdio.h>

#include "../include/errorutil.h"

void print_error(Error err) {
    if(err.type == ERROR_NONE) {
        return;
    }

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
        case ERROR_IDENT:
            printf("Unknown identifier");
            break;
    }
   
    putchar('\n');
    printf("     :%s\n", err.msg);
}
