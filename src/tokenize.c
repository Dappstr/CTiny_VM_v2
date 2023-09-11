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

    size_t token_length = token->end - token->begin;
    size_t str_length = strlen(str);

    if(token_length != str_length) {
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
    
    return *str == '\0';
}

void tokenize(Token *root, Token *token_arr, size_t num_tokens) { 
    Token* temp = root;
    int temp_val;
    
    for(int i = 0; i < num_tokens; ++i) {
        //Copy the token string into the token index
        memcpy(&token_arr[i], temp, sizeof(Token));
        if(token_str_equal("push", &token_arr[i]) || token_str_equal("pop", &token_arr[i]) || token_str_equal("print", &token_arr[i]) || token_str_equal("halt", &token_arr[i])) {
            token_arr[i].type = INST;
            if(token_str_equal("push", &token_arr[i])) {
                token_arr[i].value.inst = PUSH;
            }
            else if(token_str_equal("pop", &token_arr[i])) {
                token_arr[i].value.inst = POP;
            }
            else if(token_str_equal("print", &token_arr[i])) {
                token_arr[i].value.inst = PRINT;
            }
            else if(token_str_equal("add", &token_arr[i])) {
                token_arr[i].value.inst = ADD;
            }
            else if(token_str_equal("mult", &token_arr[i])) {
                token_arr[i].value.inst = MULT;
            }
            else if(token_str_equal("halt", &token_arr[i])) {
                token_arr[i].value.inst = HALT;
            }
        }

        else if(token_str_equal("set", &token_arr[i]) || token_str_equal("alloc", &token_arr[i]) || token_str_equal("dealloc", &token_arr[i])) {
            token_arr[i].type = FUNC;
            if(token_str_equal("set", &token_arr[i])) {
                token_arr[i].value.func = SET;
            }
            else if(token_str_equal("alloc", &token_arr[i])) {
                token_arr[i].value.func = ALLOC;
            }
            else if(token_str_equal("dealloc", &token_arr[i])) {
                token_arr[i].value.func = DEALLOC;
            }
        }

        else if(token_str_equal("var", &token_arr[i])) {
            token_arr[i].type = VAR;
            token_arr[i].value.val = 0;
        }
        
        else if(token_str_equal("#", &token_arr[i])) {
            token_arr[i].type = LIT;
            token_arr[i].value.val = 0;
        }

        else if(token_str_equal("=", &token_arr[i])) {
            token_arr[i].type = ASSNMT;
            token_arr[i].value.val = 0;
        }

        else if(token_str_equal("(", &token_arr[i])) {
            token_arr[i].type = L_PAREN;
            token_arr[i].value.val = 0;
        }

        else if(token_str_equal(")", &token_arr[i])) {
            token_arr[i].type = R_PAREN;
            token_arr[i].value.val = 0;
        }

        else if(token_str_equal(",", &token_arr[i])) {
            token_arr[i].type = COMMA;
            token_arr[i].value.val = 0;
        }

        else if((temp_val = atoi(token_arr[i].begin)) != 0) {
            token_arr[i].type = NUM;
            token_arr[i].value.val = temp_val;
        }
        
        //Token is an identifier
        else {
            token_arr[i].type = IDENT;
            size_t ident_size = token_arr[i].end - token_arr[i].begin;
            token_arr[i].value.id = malloc(ident_size);
            strncpy(token_arr[i].value.id, token_arr[i].begin, ident_size);
        }
        temp = temp->next_token;
    }
}
