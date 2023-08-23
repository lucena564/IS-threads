#include <stdio.h>
#include <unistd.h>    // Necessário para a função sleep(1).
#include <pthread.h>   // Biblioteca de acesso a Thread.

// Estrutura para passar parâmetros para uma thread
struct param_t {
    int nst;    // Numero de segundos no total
    char *nome; // Nome da thread
};

pthread_t t1; // Identificador da thread t1
pthread_t t2; // Identificador da thread t2


// Espera passar pparam->nst segundos, de segundo em segundo.

// 'pparam->nst' é uma expressão que acessa o campo 'nst' da estrutura apontada por 'pparam'.

// 'pparam': É um ponteiro para uma estrutura do tipo 'param_t'.
// '->': É o operador de acesso à memória de uma estrutura através de um ponteiro.
// 'nst': É o campo 'nst' da estrutura 'param_t' (nossa struct).

void codigo_tarefa_1(struct param_t *pparam){
    for (int ns=0; ns < pparam->nst; ++ns){      // ns menor que nst (variável que o ponteiro pparam está apontando)
                                                 // ns = 0; ns < nst (da struct apontada por pparam); ++ns
        sleep(1);
        printf("%s: passaram %d segundos\n", pparam->nome, ns+1);
    }
}

// Espera passar *nst segundos, de segundo em segundo.
void codigo_tarefa_2(int *nst){
    for(int ns=0; ns < *nst; ++ns){
        sleep(1);
        printf("Tarefa 2: passaram %d segundos\n", ns+1);
    }
}

// Função principal, cria as threads.
int main(){
    struct param_t param1; // Crio uma estrutura param_t (que foi definida lá em cima) na variável "param1"
    int param2;

    // Parametros para a thread 1
    param1.nst = 5;
    param1.nome = "TAREFA 1";

    // Parametros para a thread 2
    param2 = 15;

    printf("Inicio\n");

    // A variávei pthread_create possuem 4 parâmetros:

    // 1° parâmetro: a variável identificadora da Thread - Ta na linha 5 e 6
    // 2° parâmetro: podemos manipular parâmetros de Threads. Utilizaremos o default por enquanto.
    // 3° parâmetro: nome da função que a Thread irá executar.
    // 4° parâmetro: parâmetros para a thread.

    pthread_create(&t1, NULL, (void *) codigo_tarefa_1, (void *) &param1); // &param1 obtem o endereço da estrutura param1.
                                                                           // *param1 obtem o valor armazenado em param1.

    pthread_create(&t2, NULL, (void *) codigo_tarefa_2, (void *) &param2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Fim\n");
}