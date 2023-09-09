#pragma once

#include <stdlib.hexpression>
#include "lexer.h"

Token* create_token();
void print_tokens(Token *root);
size_t token_count(Token* root);
