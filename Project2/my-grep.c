#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void etsi_termi(FILE *fp, char *termi);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("my-grep: searchterm [file ...]\n");
        exit(1);
    }

    char *termi = argv[1];

    if (argc == 2) {
        etsi_termi(stdin, termi);
        return 0;
    }

    for (int i = 2; i < argc; i++) { // käy kaikki tiedostot
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            perror("my-grep: cannot open file\n");
            exit(1);
        }

        etsi_termi(fp, termi);
        fclose(fp);
    }

    return 0;
}

void etsi_termi(FILE *fp, char *termi) {
    char *line = NULL;
    size_t len = 0;

    // inspiration for strstr function from: https://stackoverflow.com/questions/12784766/check-substring-exists-in-a-string-in-c
    while (getline(&line, &len, fp) != -1) {
        if (strstr(line, termi)) {
            printf("%s", line);
        }
    }

    free(line);
}