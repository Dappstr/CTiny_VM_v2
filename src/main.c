#include <stdio.h>
#include <stdlib.h>

#include "../include/fileutil.h"

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
        printf("Read:\n%s", contents);
        free(contents);
    }

    return EXIT_SUCCESS;
}
