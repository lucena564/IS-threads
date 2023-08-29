#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *function(void *arg) {
  int *vetor = (int *)(arg);
  int i = 0;
  while (vetor[i] == 0 || vetor[i] == 1) {
    if (i == 0 || i == 1)
      vetor[i] = 0;
    else {
      for (int j = i + 1; vetor[j] == 0 || vetor[j] == 1; j++) {
        if (j % i == 0)
          vetor[j] = 0;
      }
    }
    i++;
  }
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
  for (int j = 0; j < tamArray; j++) {
    if (array[j] == 1) {
      printf("%d ", j);
    }
  }
  exit(0);
}
