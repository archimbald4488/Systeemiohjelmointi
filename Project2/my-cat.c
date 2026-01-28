#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc == 1) { // ei tiedostoja
        return 0;
    }

    for (int i = 1; i < argc; i++) { // käy kaikki tiedostot läpi
        FILE *fp = fopen(argv[i], "r");
        if (!fp) {
            perror("my-cat: cannot open file\n");
            exit(1);
        }

        char buffer[4096];
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            printf("%s", buffer);
        }

        fclose(fp);
    }

    return 0;
}
