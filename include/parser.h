#pragma once

#include <stdlib.h>

#include "lexer.h"
#include "errorutil.h"

typedef struct Binding {
    char* id;
    int value;
} Binding;

int get_id_value(char* id, Binding* binds, size_t num_bindings);
Error parse(Token* tokens, size_t num_tokens, Binding* binds, size_t num_bindings, int* stack);
