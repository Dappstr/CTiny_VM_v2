#pragma once

#include <stdlib.h>

#include "lexer.h"
#include "errorutil.h"

Token* create_token();
void print_tokens(Token *root);
size_t token_count(Token* root);
int token_str_equal(const char* str, Token* token);
void tokenize(Token* root, Token* token_arr, size_t token_count);
