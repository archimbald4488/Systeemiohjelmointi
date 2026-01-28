#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "punzip: file1 [file2 ...]\n");
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (!fp) {
            fprintf(stderr, "punzip: cannot open file\n");
            exit(1);
        }

        int count;
        char c;

        while (fread(&count, sizeof(int), 1, fp) == 1) {
            fread(&c, sizeof(char), 1, fp);
            for (int j = 0; j < count; j++) {
                putchar(c);
            }
        }

        fclose(fp);
    }

    return 0;
}
