#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *function(void *arg) {
  int *vetor = (int *)(arg);
  int i = 0;
  while (vetor[i] == 0 || vetor[i] == 1) {
    if (i == 0 || i == 1){
        pthread_mutex_lock(&mutex);
        vetor[i] = 0;
        pthread_mutex_unlock(&mutex);
    }
    else {
      for (int j = i + 1; vetor[j] == 0 || vetor[j] == 1; j++) {
        if (j % i == 0){
            pthread_mutex_lock(&mutex);
            vetor[j] = 0;
            pthread_mutex_unlock(&mutex);
        }
      }
    }
    i++;
  }
  pthread_exit(NULL);
}

int main() {
  int qntdThreads, tamArray;
  printf("Quantidade de Threads = ");
  scanf("%d", &qntdThreads);
  printf("Tamanho do Array = ");
  scanf("%d", &tamArray);
  int array[tamArray];
  for (int i = 0; i < tamArray; i++) {
    array[i] = 1;
  }

  pthread_t threads[qntdThreads];
  for (int i = 0; i < qntdThreads; i++) {
    pthread_create(&threads[i], NULL, function, (void *)(array));
  }
  for (int i = 0; i < qntdThreads; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("\n");
  printf("Números primos menores que %d:\n", tamArray);
  for (int j = 0; j < tamArray; j++) {
    if (array[j] == 1) {
      printf("%d ", j);
    }
  }
  exit(0);
}