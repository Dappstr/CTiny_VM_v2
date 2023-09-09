#include <stdio.h>
#include <stdlib.h>

#include "../include/fileutil.h"

unsigned int get_file_size(FILE *file) {
    if(!file) {
        printf("Failed to get file size! in %s\n", __func__);
    }
    fseek(file, 0, SEEK_END);
    unsigned int size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if(size == 0) {
        printf("Failed to get file size!\n");
        exit(EXIT_FAILURE);
    }
    else if(size == 1) {
        printf("File contains only EOF\n");
        exit(EXIT_FAILURE);
    }

    return size;
}

char* get_file_contents(const char* path) {
    FILE* file = fopen(path, "r");
    if(!file) {
        printf("Failed to get file pointer in %s\n", __func__);
        exit(EXIT_FAILURE);
    }

    unsigned int size = get_file_size(file);
    char* contents = malloc(size + 1);
    fread(contents, 1, size, file);
    contents[size] = '\0';

    fclose(file);
    return contents;
}
