#pragma once

typedef struct Error {
    enum ErrorType {
        ERROR_NONE = 0,
        ERROR_ARGUMENT,
        ERROR_SYNTAX,
        ERROR_IDENT,
    } type;
    char* msg;
} Error;

#define ERROR_PREP(n, t, m)     \
    (n).type = t;               \
    (n).msg = m;

void print_error(Error err);
