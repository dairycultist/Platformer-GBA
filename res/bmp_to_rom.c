#include <stdio.h>

// https://en.wikipedia.org/wiki/BMP_file_format

void process_bmp(FILE *out, char *path, char *c_varname) {
    
    // first, just read the headers

    FILE *in = fopen(path, "r");

    if (!in) {
        printf("Could not open %s\n", path);
    }

    char buffer[128];

    // fgets(buffer, 128, in);

    // printf("%s\n", buffer);

    fprintf(out, "unsigned char %s[] = { 49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49 };", c_varname);

    fclose(in);
}

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        printf("Did not do anything because no output file was specified as an argument.\n");
        return 1;
    }

    FILE *out = fopen(argv[1], "w");

    // filepath is relative from Makefile since that's what's executing this program
    process_bmp(out, "res/tiles.bmp", "test_tile");

    fclose(out);

    return 0;
}