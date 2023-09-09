#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/tokenize.h"
#include "../include/lexer.h"
#include "../include/errorutil.h"

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

size_t token_count(Token *root) {
    size_t num_tokens = 0; //Assume at least one token
    while(root) {
        root = root->next_token;
        ++num_tokens;
    }
    return num_tokens;
}

//Return 1 for success, 0 for failure
int token_str_equal(const char *str, Token *token) {
    if(!str || !token) {
        return 0;
    }

    char* beg = token->begin;
    while(*str && beg < token->end) {
        if(*beg != *str) {
            return 0;
        }
        beg++;
        str++;
    }
    
    return 1;
}

void tokenize(Token *root, Token *token_arr, size_t num_tokens) { 
    Token* temp = root;
    for(int i = 0; i < num_tokens; ++i) {
        //Copy the token string into the token index
        memcpy(&token_arr[i], temp, sizeof(Token));

        if(token_str_equal("var", &token_arr[i])) {
            token_arr[i].type = VAR;
        }
        temp = temp->next_token;
    }
}
