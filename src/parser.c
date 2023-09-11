#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <assert.h>

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
    size_t nums_allocated = 0;
    for(int i = 0; i < num_tokens; ++i) {
        if(tokens[i].type == INST) {
            switch(tokens[i].value.inst) {
                case PRINT:
                    printf("[ ");
                    for(int i = 0; i < nums_allocated; ++i) {
                        if(i == nums_allocated - 1 || nums_allocated == 0 || nums_allocated == 1) {
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
                        else if(nums_pushed > nums_allocated) {
                            printf("Push index greater than allocated amount of numbers!\n");
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
                        int val_to_push;
                        if(tokens[i+1].type == LIT) {
                            if(tokens[i+2].type != NUM) {
                                ERROR_PREP(err, ERROR_SYNTAX, "Expected number after '#'");
                                return err;
                            }
                            else {
                                val_to_push = tokens[i+2].value.val;
                                i+=2;
                            }
                        }
                        else {
                            char* id = malloc(tokens[i+1].end - tokens[i+1].begin);
                            strncpy(id, tokens[i+1].begin, tokens[i+1].end - tokens[i+1].begin);
                            val_to_push = get_id_value(id, binds, num_binds);
                            free(id);
                            if(val_to_push == INT_MIN) {
                                ERROR_PREP(err, ERROR_SYNTAX, "Unknown identifier");
                                return err;
                            }
                            i+=1;
                        }
                        if(nums_pushed >= nums_allocated) {
                            stack = realloc(stack, sizeof(int) * (nums_allocated + 1));
                            if(!stack) {
                                printf("Failed to reallocate memory for another integer\n");
                                exit(EXIT_FAILURE);
                            }
                            nums_allocated++;
                        }
                        stack[nums_pushed] = val_to_push;
                        ++nums_pushed;
                        break;
                    }
                case POP: {
                    if(nums_allocated < 1) {
                        free(stack);
                        stack = NULL;
                    }
                    else {
                        printf("%d popped\n", stack[nums_allocated - 1]); 
                        if(nums_allocated > 0) {
                            stack = realloc(stack, sizeof(int) * (nums_allocated - 1)); 
                            if(!stack) {
                                printf("Failed to reallocate stack after pop\n");
                                exit(EXIT_FAILURE);
                            }

                            nums_allocated--;
                            nums_pushed--;
                        }
                    }
                    break;
                }
                case ADD: {
                    int total = 0;
                    for(int x = 0; x < nums_pushed; ++x) {
                        total += stack[x];
                    }
                    printf("%d", total);
                }
                case MULT: {
                    if(nums_pushed == 0) {
                        break;
                    }
                    else if(nums_pushed == 1) {
                        printf("%d\n", stack[0]);
                    }
                    else {
                        int total = stack[0];
                        for(int x = 1; x < nums_pushed; ++x) {
                            total *= stack[x];
                        }
                        printf("%d\n", total);
                    }
                }
                case HALT:
                    ERROR_PREP(err, ERROR_NONE, NULL);
                    return err;
            }
        }
        else if(tokens[i].type == FUNC) {
            switch(tokens[i].value.func) {
                case SET: {
                        if(tokens[i+1].type != L_PAREN) {
                            ERROR_PREP(err, ERROR_SYNTAX, "Expected '(' after \"set\"");
                            return err;
                        }
                        else {
                            int indx = tokens[i+2].value.val;
                            if(indx >= nums_pushed) {
                                printf("\"set\" index is greater than amount of numbers pushed!\n");
                                exit(EXIT_FAILURE);
                            }
                            else {
                                if(tokens[i+3].type != COMMA) {
                                    ERROR_PREP(err, ERROR_SYNTAX, "Expected ',' after set index");
                                    return err;
                                }
                                else {
                                    int value = tokens[i+4].value.val;
                                    if(tokens[i+5].type != R_PAREN) {
                                        ERROR_PREP(err, ERROR_SYNTAX, "Expected ')' after set value");
                                        return err;
                                    }
                                    else {
                                        assert(indx < nums_pushed && "indx greater than nums pushed!");
                                        stack[indx] = value;
                                        i+=5;
                                    }
                                }
                            }
                        }
                        break;
                  }
                  case ALLOC:
                          if(tokens[i+1].type != L_PAREN) {
                              ERROR_PREP(err, ERROR_SYNTAX, "Expected '(' after \"dealloc\"");
                              return err;
                          }
                          else {
                              if(tokens[i+2].type != NUM) {
                                  ERROR_PREP(err, ERROR_SYNTAX, "Expected literal after '('");
                                  return err;
                              }
                              else {
                                  if(tokens[i+3].type != R_PAREN) {
                                      ERROR_PREP(err, ERROR_SYNTAX, "Expected ')' after number literal");
                                      return err;
                                  }
                                  else {
                                      int alloc_num = tokens[i+2].value.val;
                                      stack = realloc(stack, sizeof(int) * (nums_allocated + alloc_num));
                                      memset(stack + nums_pushed, 0, alloc_num);
                                      nums_allocated += alloc_num;
                                  }
                              }
                          }
                          i += 3;
                          break; 
                  case DEALLOC:
                          if(tokens[i+1].type != L_PAREN) {
                              ERROR_PREP(err, ERROR_SYNTAX, "Expected '(' after \"dealloc\"");
                              return err;
                          }
                          else {
                              if(tokens[i+2].type != NUM) {
                                  ERROR_PREP(err, ERROR_SYNTAX, "Expected literal after '('");
                                  return err;
                              }
                              else {
                                  if(tokens[i+3].type != R_PAREN) {
                                      ERROR_PREP(err, ERROR_SYNTAX, "Expected ')' after number literal");
                                      return err;
                                  }
                                  else {
                                      int dealloc_num = tokens[i+2].value.val;
                                      if(dealloc_num > nums_allocated) {
                                          printf("Error! Deallocation amount is greater than allocated amount of integers!\n");
                                          exit(EXIT_FAILURE);
                                      }
                                      else if(dealloc_num == nums_allocated) {
                                          free(stack);
                                          nums_pushed = 0;
                                          nums_allocated = 0;
                                          stack = NULL;
                                      }
                                      else {
                                          if(nums_allocated - dealloc_num < nums_pushed) {
                                              nums_pushed = nums_allocated - dealloc_num;
                                          }
                                          stack = realloc(stack, sizeof(int) * (nums_allocated - dealloc_num));
                                          if(!stack) {
                                              printf("Error reallocating stack after deallocation!\n");
                                              exit(EXIT_FAILURE);
                                          }
                                          nums_allocated -= dealloc_num;
                                      }
                                  }
                              }
                          }
                          i += 3;
                          break;
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
