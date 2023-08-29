#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // mutex para exclusão mútua
pthread_mutex_t write_mutex = PTHREAD_MUTEX_INITIALIZER; // mutex para controle das escritoras
pthread_cond_t write_condition = PTHREAD_COND_INITIALIZER; // variável de condição para escrita
pthread_cond_t read_condition = PTHREAD_COND_INITIALIZER; // variável de condição para leitura
int writers_waiting = 0; // contador de escritores esperando

int *database; // array simulando o banco de dados
int array_size; // tamanho do array
int num_readers; // número de threads leitoras
int num_writers; // número de threads escritoras

void *reader(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (writers_waiting > 0) {
            pthread_cond_wait(&read_condition, &mutex); // aguarda enquanto há escritoras esperando
        }
        pthread_mutex_unlock(&mutex);
        
        int index = rand() % array_size;
        printf("Reader %ld reads: %d\n", (long)arg, database[index]);
        
        pthread_mutex_lock(&mutex);
        pthread_cond_broadcast(&read_condition); // notifica as outras leitoras
        pthread_mutex_unlock(&mutex);
        
        usleep(100000); // atraso (em microssegundos)
    }
    return NULL;
}

void *writer(void *arg) {
    while (1) {
        int index = rand() % array_size;
        int value = rand() % 100; 
        
        pthread_mutex_lock(&write_mutex);
        database[index] = value; 
        printf("Writer %ld writes: %d at index %d\n", (long)arg, value, index);
        pthread_mutex_unlock(&write_mutex);
        
        usleep(200000); // atraso (em microssegundos)
    }
    return NULL;
}

int main() {
    printf("Enter the array size, number of readers, and number of writers: ");
    scanf("%d %d %d", &array_size, &num_readers, &num_writers);

    database = (int *)malloc(sizeof(int) * array_size); // aloca o array do banco de dados

    pthread_t readers[num_readers]; // array threads leitoras
    pthread_t writers[num_writers]; // array threads escritoras

    srand(time(NULL)); // gerar números aleatórios

    // cria as threads leitoras
    for (long i = 0; i < num_readers; i++) {
        pthread_create(&readers[i], NULL, reader, (void *)i);
    }

    // cria as threads escritoras
    for (long i = 0; i < num_writers; i++) {
        pthread_create(&writers[i], NULL, writer, (void *)i);
    }

    // aguarda o término das threads leitoras
    for (int i = 0; i < num_readers; i++) {
        pthread_join(readers[i], NULL);
    }

    // aguarda o término das threads escritoras
    for (int i = 0; i < num_writers; i++) {
        pthread_join(writers[i], NULL);
    }

    free(database); // libera a memória alocada do database
    return 0;
}
