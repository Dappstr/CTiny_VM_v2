#pragma once

#include "errorutil.h"

typedef enum TokenType {
    INST = 0,
    LIT,
    NUM,
    VAR,
    IDENT,
    ASSNMT,
    FUNC,
    L_PAREN,
    R_PAREN,
    COMMA,
} TokenType;

typedef enum InstType {
    PUSH,
    POP,
    PRINT,
    ADD,
    MULT,
    HALT,
} InstType;

typedef enum FuncType {
    SET,
    ALLOC, //TODO
    DEALLOC, //TODO
} FuncType;

typedef struct Token {
    TokenType type;
    char* begin;
    char* end;

    union TokenValue {
        int val;
        char* id;
        InstType inst;
        FuncType func;
    } value;

    struct Token* next_token;
} Token;

Error lex(char* source, Token* token);

