#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/tokenize.h"
#include "../include/lexer.h"

Token* create_token() {
    Token* token = malloc(sizeof(Token));
    if(!token) {
        printf("Failed to allocate memory for a token\n");
        exit(EXIT_FAILURE);
    }

    memset(token, 0, sizeof(Token));
    return token;
}

void print_tokens(Token *root) {
    size_t num_tokens = 1;
    while(root) {
        printf("Token %zu: %.*s\n", num_tokens, (int)(root->end - root->begin), root->begin);
        ++num_tokens;
        root = root->next_token;
    }
}
