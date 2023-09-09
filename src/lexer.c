#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/lexer.h"
#include "../include/errorutil.h"

const char* delims = " \r\n()#=";
const char* ws = " \r\n";

Error lex(char* source, Token* token) {
    //Start off with no error
    Error err = {ERROR_NONE, NULL};

    if(!source || !token) {
        ERROR_PREP(err, ERROR_ARGUMENT, "Invalid source");
        return err;
    }

    token->begin = source;
    token->begin += strspn(token->begin, ws);

    token->end = token->begin;
    if(*(token->end) == '\0') { return err; }
    
    token->end += strcspn(token->begin, delims);
    
    if(token->begin == token->end) {
        token->end += 1;
    }

    return err;
}

