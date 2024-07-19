//tri bulles puis fusion en 4 threads

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

#define N 100000


#define NB_THREADS 2

typedef struct
{
    int *tab;
    int debut;
    int fin;
} param_t;


// teste si le tableau est bien trié
int est_trie(int *tab, int taille)
{
    int i;
    for (i = 0; i < taille - 1; i++)
    {
        if (tab[i] > tab[i + 1])
            return 0;
    }
    return 1;
}

//tri à bulles
void *tri_bulles(void *arg)
{
    param_t *param = (param_t *)arg;
    int *tab = param->tab;

    int i, j, tmp;
    for (i = param->debut; i < param->fin; i++)
    {
        for (j = param->debut; j < param->fin-i+param->debut; j++)
        {
            if (tab[j] > tab[j + 1])
            {
                tmp = tab[j];
                tab[j] = tab[j + 1];
                tab[j + 1] = tmp;
            }
        }
    }
    pthread_exit(NULL);
}

//fusion de deux tableaux triés
void fusion(int *tab, int taille)
{
    int *copie = (int *)malloc(taille * sizeof(int));
    memcpy(copie, tab, taille * sizeof(int));

    int milieu = taille / 2;
    int i = 0, j=0, k=0;
    while (i < milieu && j < milieu)
        if (copie[i] <= copie[milieu + j])
            tab[k++] = copie[i++];
        else
            tab[k++] = copie[milieu + j++];
    while (i < milieu)
        tab[k++] = copie[i++];
    while (j < milieu)
        tab[k++] = copie[milieu + j++];
    free(copie);
}

//main
int main()
{
    int i, j, tmp;
    int tab[N];
    param_t param[NB_THREADS];

    pthread_t threads[NB_THREADS];
    struct timeval tv1, tv2;
    double tps;
    srand(1);
    for (i = 0; i < N; i++)
    {
        //tab[i] = rand() % (N*10);
        tab[i] = N - i;
    }
    gettimeofday(&tv1, NULL);
    for (i = 0; i < NB_THREADS; i++)
    {
        param[i].tab = tab;
        param[i].debut = i * N / NB_THREADS;
        param[i].fin = (i + 1) * N / NB_THREADS-1;
    }

    for (i = 0; i < NB_THREADS; i++)
        pthread_create(&threads[i], NULL, tri_bulles, (void *)&param[i]);

    for (i = 0; i < NB_THREADS; i++)
        pthread_join(threads[i], NULL);

    if (NB_THREADS==2) fusion(tab, N);
    gettimeofday(&tv2, NULL);
    tps = (double)(tv2.tv_usec - tv1.tv_usec) / 1000000 + (double)(tv2.tv_sec - tv1.tv_sec);
    printf("temps tri bulles : %f\n", tps);

    printf("est trié : %d\n", est_trie(tab, N));
    return 0;
}