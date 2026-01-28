#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sysinfo.h>

/*This program took a lot of work and I have taken inspiration from multiple sites, which I will collect here. 
The main contributors to this code are various stackoverflow posts that helped me create the structure for the threads,
understand how to combine the files into one stream with mmap, and setup the pthreads themselves.

Links:
https://stackoverflow.com/questions/52767944/merge-sort-with-pthreads-in-c
https://stackoverflow.com/questions/20480529/thread-struct-as-function-parameter-c
https://stackoverflow.com/questions/3512434/using-struct-stat
https://ramcdougal.com/threads.html
https://blog.minhazav.dev/memory-sharing-in-linux/
*/

typedef struct {
    char *data;
    long alku;
    long loppu;
    int id;
} task_t;

typedef struct {
    int *maarat;
    char *merkit;
    int koko;
} result_t;

result_t *tulokset;

void *kompressoi(void *arg);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "pzip: file1 [file2 ...]\n");
        exit(1);
    }

    // mmap all files
    long total_koko = 0;
    char **maps = malloc((argc-1) * sizeof(char*));
    long *sizes = malloc((argc-1) * sizeof(long));

    for (int i = 1; i < argc; i++) {
        int fd = open(argv[i], O_RDONLY);
        if (fd < 0) {
            fprintf(stderr, "pzip: cannot open file\n");
            exit(1);
        }

        struct stat st; // use stat to capture information
        fstat(fd, &st);
        sizes[i-1] = st.st_size;
        total_koko += st.st_size;

        maps[i-1] = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
        close(fd);
    }

    // combine into one logical stream
    char *data = malloc(total_koko);
    long offset = 0;
    for (int i = 0; i < argc-1; i++) {
        memcpy(data + offset, maps[i], sizes[i]);
        munmap(maps[i], sizes[i]);
        offset += sizes[i];
    }
    free(maps);
    free(sizes);

    // thread setup
    int nthreads = get_nprocs();

    if (total_koko == 0) {
        free(data);
        return 0;
    }   // tarkistuksia jottei mm. jaeta nollalla myöhemmin
    if (nthreads > total_koko) nthreads = total_koko;
    if (nthreads < 1) nthreads = 1;

    pthread_t threads[nthreads];
    task_t tasks[nthreads];
    tulokset = malloc(nthreads * sizeof(result_t));

    long chunk = total_koko / nthreads;

    for (int i = 0; i < nthreads; i++) {
        tasks[i].alku = i * chunk;
        tasks[i].loppu = (i == nthreads - 1) ? total_koko : (i+1)*chunk;
        tasks[i].data = data;
        tasks[i].id = i;
        pthread_create(&threads[i], NULL, kompressoi, &tasks[i]);
    }


    for (int i = 0; i < nthreads; i++)
        pthread_join(threads[i], NULL);

        // yhdistetään tulokset!

    int eka = 1;
    int e_luku = 0;
    char e_merkki = 0;

    for (int i = 0; i < nthreads; i++) {
        for (int j = 0; j < tulokset[i].koko; j++) {
            int c = tulokset[i].maarat[j];
            char ch = tulokset[i].merkit[j];

            if (eka) {
                e_luku = c;
                e_merkki = ch;
                eka = 0;
            } else if (ch == e_merkki) {
                e_luku += c;
            } else {
                fwrite(&e_luku, sizeof(int), 1, stdout);
                fwrite(&e_merkki, sizeof(char), 1, stdout);
                e_luku = c;
                e_merkki = ch;
            }
        }
    }

    if (!eka) {
        fwrite(&e_luku, sizeof(int), 1, stdout);
        fwrite(&e_merkki, sizeof(char), 1, stdout);
    }

    // muistetaan muistinvapautus
    for (int i = 0; i < nthreads; i++) {
        free(tulokset[i].maarat);
        free(tulokset[i].merkit);
    }
    free(tulokset);
    free(data);
    return 0;
}


void *kompressoi(void *arg) {
    task_t *task = (task_t *)arg;
    result_t *tulos = &tulokset[task->id];

    int raja = 1024;
    tulos->maarat = malloc(raja * sizeof(int));
    tulos->merkit  = malloc(raja * sizeof(char));
    tulos->koko = 0;

    if (task->alku >= task->loppu) return NULL;

    char edellinen = task->data[task->alku];
    int maara = 1;

    // kompressointi tehty samalla periaattella kuin 2. projektissa
    // muutamia muutoksia, kuten muistinhallinta kun nyt pitää tallentaa eikä vain tulostaa
    for (long i = task->alku + 1; i < task->loppu; i++) {
        if (task->data[i] == edellinen) {
            maara++;
        } else {
            if (tulos->koko >= raja) {
                raja *= 2;
                tulos->maarat = realloc(tulos->maarat, raja * sizeof(int));
                tulos->merkit  = realloc(tulos->merkit, raja * sizeof(char));
            }
            tulos->maarat[tulos->koko] = maara;
            tulos->merkit[tulos->koko]  = edellinen;
            tulos->koko++;
            edellinen = task->data[i];
            maara = 1;
        }
    }

    // inhoan osoitinmuuttujia
    tulos->maarat[tulos->koko] = maara;
    tulos->merkit[tulos->koko]  = edellinen;
    tulos->koko++;
    return NULL;
}
