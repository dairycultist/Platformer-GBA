#include <stdio.h>

// https://en.wikipedia.org/wiki/BMP_file_format

// first, just read the headers

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        printf("Did not do anything because no output file was specified as an argument.\n");
        return 1;
    }

    FILE *file;

    file = fopen(argv[1], "w");

    fprintf(file, "Some text");

    // Close the file
    fclose(file);

    return 0;
}