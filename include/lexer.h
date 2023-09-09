#pragma once

#include "errorutil.h"

typedef enum TokenType {
    INST = 0,
    LIT,
    NUM,
    VAR,
    IDENT,
    ASSNMT,
} TokenType;

typedef enum InstType {
    PUSH,
    PRINT,
    HALT,
} InstType;

typedef struct Token {
    TokenType type;
    char* begin;
    char* end;

    union TokenValue {
        int val;
        InstType inst;
    } value;
    struct Token* next_token;
} Token;

Error lex(char* source, Token* token);

