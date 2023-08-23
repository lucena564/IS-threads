#include <stdio.h>
#include <unistd.h>
#include <pthread.h>   // Biblioteca de acesso a Thread

pthread_t t1;          // Identificador da thread t1
pthread_t t2;          // Identificador da thread t2

// Função da tarefa 1: espera passar 10 segundos, exibindo o tempo decorrido a cada segundo.

void tarefa1(void){
    for (int segundos = 0; segundos < 10; ++segundos){
        sleep(1);
        printf("Tarefa 1: Passaram %d segundos\n", segundos + 1);
    }
}

// Função da tarefa 2: espera passar 15 segundos, exibindo o tempo decorrido a cada segundo.

void tarefa2(void){
    for (int segundos = 0; segundos < 15; ++segundos){
        sleep(1);
        printf("Tarefa 2: Passaram %d segundos\n", segundos + 1);
    }
}

int main(void) {
    printf("Início\n");

    // A variávei pthread_create possuem 4 parâmetros:

    // 1° parâmetro: a variável identificadora da Thread - Ta na linha 5 e 6
    // 2° parâmetro: podemos manipular parâmetros de Threads. Utilizaremos o default por enquanto.
    // 3° parâmetro: nome da função que a Thread irá executar.
    // 4° parâmetro: podemos passar parâmetros para a thread. Agora não iremos passar nenhum parâmetro,
    //               na aula2.c iremos.

    pthread_create(&t1, NULL, (void *) tarefa1, NULL); 
    pthread_create(&t2, NULL, (void *) tarefa2, NULL);

    pthread_join(t1, NULL); // Precisamos dar o tempo necessário para as Threads t1 e t2 acabem suas execuções;
    pthread_join(t2, NULL); // Esse tempo é dado por essa variável.

    // ---------------------------- Perceba que se eu fizesse isso aqui: ----------------------------

    // pthread_create(&t1, NULL, (void *) tarefa1, NULL); 
    // pthread_join(t1, NULL); 

    // pthread_create(&t2, NULL, (void *) tarefa2, NULL);
    // pthread_join(t2, NULL);

    // Eu estaria fazendo uma execução linear, não seria feita uma execução simultânea

    // ----------------------------------------------------------------------------------------------

    printf("Fim\n");
}