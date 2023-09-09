#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/fileutil.h"
#include "../include/errorutil.h"
#include "../include/lexer.h"
#include "../include/tokenize.h"

void print_usage(const char* argv) {
    printf("USAGE: %s <path to file>", argv);
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* path = argv[1];
    char* contents = get_file_contents(path);

    if(contents) {
        //Lex & Tokenize

        Error ok = {ERROR_NONE, NULL};
        Error err = ok;

        Token* tokens = NULL;
        Token* token_it = tokens;

        Token current_token;
        current_token.begin = contents;
        current_token.end = contents;
        current_token.next_token = NULL;

        while((err = lex(current_token.end, &current_token)).type == ERROR_NONE) {
            size_t token_size = current_token.end - current_token.begin;
            if(token_size == 0) { break; }
            
            if(tokens) {
                token_it->next_token = create_token();
                memcpy(token_it->next_token, &current_token, sizeof(Token));
                token_it = token_it->next_token;
            }
            else {
                tokens = create_token();
                memcpy(tokens, &current_token, sizeof(Token));
                token_it = tokens;
            }
        }

        print_tokens(tokens);

        size_t num_tokens = token_count(tokens);
        Token* token_arr = malloc(sizeof(Token) * num_tokens);
        
        tokenize(tokens, token_arr, num_tokens);

        //Parse
       /*
        *err = parse(tokens, num_tokens, binds);
        *print_error(err);
        */

        free(contents);
    }

    return EXIT_SUCCESS;
}
