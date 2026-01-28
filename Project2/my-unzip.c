#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("my-unzip: file1 [file2 ...]\n");
        exit(1);
    }

    for (int i = 1; i < argc; i++) { // käy kaikki tiedostot
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            perror("my-unzip: cannot open file\n");
            exit(1);
        }

        int maara;
        char c;

        while (fread(&maara, sizeof(int), 1, fp) == 1 && fread(&c, sizeof(char), 1, fp) == 1) {
            for (int j = 0; j < maara; j++) {
                printf("%c", c);
            }
        }

        fclose(fp);
    }

    return 0;
}
