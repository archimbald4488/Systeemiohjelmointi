#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // assume stdin and stdout with 'fake' files for no command line arguments
    FILE *in = stdin;
    FILE *out = stdout;

    // check for the command line arguments
    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    if (argc >= 2) {
        in = fopen(argv[1], "r");
        if (!in) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
    }

    if (argc == 3) {
        if (strcmp(argv[1], argv[2]) == 0) {
            fprintf(stderr, "Input and output file must differ\n");
            exit(1);
        }

        out = fopen(argv[2], "w");
        if (!out) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
            exit(1);
        }
    }

    // read the input
    // inspiration for reallocation and pointer pointer (array of strings) from: https://www.wscubetech.com/resources/c-programming/programs/reverse-array
    char **lines = NULL;
    size_t maara = 0; // rivitaulukon (lines) indeksi sekä rivien määrä
    size_t suuruus = 0; // taulukon suuruus

    char *buffer = NULL;
    size_t len = 0;

    while (getline(&buffer, &len, in) != -1) {
        if (maara >= suuruus) {
         suuruus = suuruus == 0 ? 10 : suuruus * 2;
            char **temp = realloc(lines, suuruus * sizeof(char *));
            if (!temp) {
                fprintf(stderr, "malloc failed\n");
                exit(1);
            }
            lines = temp;
        }

        lines[maara] = strdup(buffer);
        if (!lines[maara]) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        maara++;
    }

    free(buffer);
    fclose(in);


    // reverse and print
    for (ssize_t i = maara - 1; i >= 0; i--) {
        fprintf(out, "%s", lines[i]);
        free(lines[i]);
    }

    free(lines);

    if (out != stdout)
        fclose(out);

    return 0;
}
