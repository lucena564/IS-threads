#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //Criação de Mutex para controle de concorrência entre as Threads.

//Função que executará o crivo de Eratóstenes:
void *function(void *arg) {
  int *vetor = (int *)(arg);
  int i = 0;
  while (vetor[i] == 0 || vetor[i] == 1) {
    if (i == 0 || i == 1){ //Passa 0 para os 2 primeiros espaços do vetor de primos (espaço 0 e 1).
        pthread_mutex_lock(&mutex);
        vetor[i] = 0;
        pthread_mutex_unlock(&mutex);
    }
    else {
      for (int j = i + 1; vetor[j] == 0 || vetor[j] == 1; j++) {//Aplicação do algoritmo no resto do vetor.
        if (j % i == 0){ //Caso encontre algum valor múltiplo daquele primo dentro do vetor, será passado para 0.
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
  //Declaração das variáveis que serão utilizadas
  int qntdThreads, tamArray;//QtndThreads é a quantidade de Threads na qual o usuário passará e TamArray é o tamanho do array de booleano.

  //Algoritmo de seleção da quantidade de threads do sistema e do tamanho do array feita pelo usuário.
  printf("Quantidade de Threads = ");
  scanf("%d", &qntdThreads);
  printf("Tamanho do Array = ");
  scanf("%d", &tamArray);
  int array[tamArray]; //Criação do array de booleano com o tamanho = tamArray.
  for (int i = 0; i < tamArray; i++) {//Seta todos os valores do array para 1 inicialmente, assim como dito na questão.
    array[i] = 1;
  }

  pthread_t threads[qntdThreads]; //Cria um array de Threads com o tamanho = qntdThreads (Definida anteriormente pelo usuário).

  for (int i = 0; i < qntdThreads; i++) {//Irá percorrer todo o array de threads e criar uma thread para cada espaço do array (que tem tamanho definido pelo usuário = qntdThreads).
    pthread_create(&threads[i], NULL, function, (void *)(array));
  }
  for (int i = 0; i < qntdThreads; i++) {
    pthread_join(threads[i], NULL);//Junta as threads criadas com a thread principal do sistema após todas as threads terem terminado o que tinham que fazer
  }

  printf("\n");
  printf("Números primos menores que %d:\n", tamArray);
  for (int j = 0; j < tamArray; j++) { //Irá printar os espaços que restaram setados (ou seja, = 1) no array de booleanos, que serão apenas os espaços primos < tamArray.
    if (array[j] == 1) {
      printf("%d ", j);
    }
  }
  exit(0);
}