#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("my-zip: file1 [file2 ...]\n");
        exit(1);
    }

    int merkki = EOF;
    int maara = 0;

    for (int i = 1; i < argc; i++) { // käy kaikki tiedostot
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            perror("my-zip: cannot open file\n");
            exit(1);
        }

        // inspiration for reading individual characters from: https://stackoverflow.com/questions/4823177/reading-a-file-character-by-character-in-c
        int c;
        while ((c = fgetc(fp)) != EOF) {
            if (merkki == EOF) {
                merkki = c;
                maara = 1;
            } else if (c == merkki) {
                maara++;
            } else {
                // sizeof(int) and sizeof(char) should ensure we get the correct 5-byte format
                fwrite(&maara, sizeof(int), 1, stdout); 
                fwrite(&merkki, sizeof(char), 1, stdout);
                merkki = c;
                maara = 1;
            }
        }

        fclose(fp);
    }

    if (merkki != EOF) {
        fwrite(&maara, sizeof(int), 1, stdout);
        fwrite(&merkki, sizeof(char), 1, stdout);
    }

    return 0;
}
