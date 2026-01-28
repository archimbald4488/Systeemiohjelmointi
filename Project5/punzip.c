#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/sysinfo.h>

typedef struct {
    int *maarat;
    char *merkit;
    long start;
    long end;
    int id;
} task_t;

typedef struct {
    char *out;
    long koko;
} result_t;

result_t *tulokset;

void *dekompressoi(void *arg);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "punzip: file1 [file2 ...]\n");
        exit(1);
    }

    // lue kaikki kompressoitu data
    int *maarat = NULL;
    char *merkit = NULL;
    long raja = 1024, koko = 0;

    maarat = malloc(raja * sizeof(int));
    merkit  = malloc(raja * sizeof(char));

    for (int f = 1; f < argc; f++) {
        int fd = open(argv[f], O_RDONLY);
        if (fd < 0) {
            fprintf(stderr, "punzip: cannot open file\n");
            exit(1);
        }

        struct stat st;
        fstat(fd, &st);

        char *map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
        close(fd);

        long entries = st.st_size / 5;
        for (long i = 0; i < entries; i++) {
            if (koko >= raja) {
                raja *= 2;
                maarat = realloc(maarat, raja * sizeof(int));
                merkit  = realloc(merkit,  raja * sizeof(char));
            }
            int c;
            memcpy(&c, map + i*5, sizeof(int));
            maarat[koko] = c;
            merkit[koko]  = *(map + i*5 + 4);
        }
        munmap(map, st.st_size);
    }

    if (koko == 0) {
        free(maarat);
        free(merkit);
        return 0;
    }

    // jaa työ threadeille
    int nthreads = get_nprocs();

    if (nthreads > koko) nthreads = koko;
    if (nthreads < 1) nthreads = 1;

    pthread_t threads[nthreads];
    task_t tasks[nthreads];
    tulokset = malloc(nthreads * sizeof(result_t));

    long chunk = koko / nthreads;

    for (int i = 0; i < nthreads; i++) {
        tasks[i].start = i * chunk;
        tasks[i].end   = (i == nthreads - 1) ? koko : (i+1)*chunk;
        tasks[i].maarat = maarat;
        tasks[i].merkit  = merkit;
        tasks[i].id = i;
        pthread_create(&threads[i], NULL, dekompressoi, &tasks[i]);
    }

    for (int i = 0; i < nthreads; i++)
        pthread_join(threads[i], NULL);

    // output in order?
    for (int i = 0; i < nthreads; i++) {
        fwrite(tulokset[i].out, 1, tulokset[i].koko, stdout);
        free(tulokset[i].out);
    }

    free(tulokset);
    free(maarat);
    free(merkit);
    return 0;
}

void *dekompressoi(void *arg) {
    task_t *t = (task_t *)arg;
    result_t *tulos = &tulokset[t->id];

    long total = 0;
    for (long i = t->start; i < t->end; i++)
        // printf("running on: ",)
        total += t->maarat[i]; // täällä menee jotain väärin?

    tulos->out = malloc(total);
    tulos->koko = total;

    long pos = 0;
    for (long i = t->start; i < t->end; i++) {
        for (int j = 0; j < t->maarat[i]; j++)
            tulos->out[pos++] = t->merkit[i];
    }
    return NULL;
}

