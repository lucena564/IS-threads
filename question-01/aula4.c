#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

// Criando Threads em um Laço em C

#define N_THREADS 10

pthread_t minhas_threads[N_THREADS];

void codigo_tarefa(int *nst){
    for (int ns=0; ns < *nst; ++ns){
        sleep(1);
        printf("Tarefa: passaram %d segundos do total de %d.\n", ns+1, *nst);
    }
}

int main(void){
    int param_threads[N_THREADS];

    printf("Inicio\\n");

    for( int i=0; i<N_THREADS; ++i)
        param_threads[i] = 5+1;
    

    for( int i=0; i<N_THREADS; ++i)
        pthread_create(&minhas_threads[i], NULL, (void *) codigo_tarefa, (void *)&param_threads[i]);

    for( int i=0; i<N_THREADS; ++i)
        pthread_join(minhas_threads[i], NULL);


    printf("Fim\\n");

    return(0);

}