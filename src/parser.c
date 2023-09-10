#include <stdio.h>
#include <limits.h>
#include <string.h>

#include "../include/parser.h"
#include "../include/errorutil.h"
#include "../include/lexer.h"

int get_id_value(char *id, Binding* binds, size_t num_binds) {
    for(int i = 0; i < num_binds; ++i) {
        if(strcmp(id, binds[i].id) == 0) {
            return binds[i].value;
        }
    }
    return INT_MIN;
}

Error parse(Token *tokens, size_t num_tokens, Binding *binds, size_t num_binds, int *stack) {
    Error err = {ERROR_NONE, NULL};
    size_t nums_pushed = 0;
    for(int i = 0; i < num_tokens; ++i) {
        if(tokens[i].type == INST) {
            switch(tokens[i].value.inst) {
                case PRINT:
                    printf("[ ");
                    for(int i = 0; i < nums_pushed; ++i) {
                        if(i == nums_pushed - 1 || nums_pushed == 0 || nums_pushed == 1) {
                            printf("%d ]\n", stack[i]);
                        }
                        else {
                            printf("%d, ", stack[i]);
                        }
                    }
                    break;
                case PUSH:
                    if(nums_pushed == 0 || stack == NULL) {
                        stack = malloc(sizeof(int) * (nums_pushed + 1));
                        if(!stack) {
                            printf("Failed to allocate memory for stack");
                            exit(EXIT_FAILURE);
                        }
                        else {
                            stack = realloc(stack, sizeof(int) * (nums_pushed + 1));
                            if(!stack) {
                                printf("Failed to reallocate memory for another integer\n");
                                exit(EXIT_FAILURE);
                            }
                        }
                    }
                    if(tokens[i+1].type != LIT && tokens[i+1].type != IDENT) {
                        ERROR_PREP(err, ERROR_SYNTAX, "Expected literal or identifer after push\n");
                        return err;
                    }
                    else {
                        if(tokens[i+1].type == LIT) {
                            if(tokens[i+2].type != NUM) {
                                ERROR_PREP(err, ERROR_SYNTAX, "Expected number after '#'");
                                return err;
                            }
                            else {
                                int val_to_push = tokens[i+2].value.val;
                                stack[nums_pushed] = val_to_push;
                                i+=2;
                            }
                        }
                        else {
                            char* id = malloc(tokens[i+1].end - tokens[i+1].begin);
                            strncpy(id, tokens[i+1].begin, tokens[i+1].end - tokens[i+1].begin);
                            int val_to_push = get_id_value(id, binds, num_binds);
                            if(val_to_push == INT_MIN) {
                                ERROR_PREP(err, ERROR_SYNTAX, "Unknown identifier");
                                return err;
                            }
                            stack[nums_pushed] = val_to_push;
                            i+=1;
                        }
                        ++nums_pushed;
                        break;
                    }
                case HALT:
                    ERROR_PREP(err, ERROR_NONE, NULL);
                    return err;
            }
        }
        else if(tokens[i].type == VAR) {
            if(tokens[i+1].type != IDENT) {
                ERROR_PREP(err, ERROR_SYNTAX, "Expected an identifier after 'var'");
                return err;
            }
            else {
                if(tokens[i+2].type != ASSNMT) {
                    ERROR_PREP(err, ERROR_SYNTAX, "Expected '=' after identifier");
                    return err;
                }
                else {
                    if(num_binds < 1) {
                        binds = malloc(sizeof(Binding) * (num_binds + 1));
                        char* id = malloc(tokens[i+1].end - tokens[i+1].begin);
                        strncpy(id, tokens[i+1].begin, tokens[i+1].end - tokens[i+1].begin);
                        binds[0].id = id;
                        binds[0].value = tokens[i+3].value.val;
                        num_binds++;
                        i+=3;
                    }
                    else {
                        binds = realloc(binds, sizeof(Binding) * (num_binds + 1));
                        binds[num_binds].id = tokens[i+1].begin;
                        binds[num_binds].value = tokens[i+3].value.val;
                        num_binds++;
                        i+=3;
                    }
                }
            }
        }
    }
    return err;
}
