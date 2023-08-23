#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

// Receber o código de término de uma thread é uma parte importante da programação com threads em C.
// Ele permite que você saiba o resultado de uma tarefa executada por uma thread, o que é útil para
// monitorar o sucesso ou falha de operações concorrentes.

// No exemplo de código fornecido, duas threads são criadas, cada uma executando uma tarefa diferente. 
// Cada tarefa dorme por um certo número de segundos, imprimindo uma mensagem a cada segundo que passa.
// A diferença entre as duas tarefas é a maneira como elas terminam.

//      * A função "codigo_tarefa_1" retorna o código de sucesso (SUCESSO_TH).
//      * A função "codigo_tarefa_2" termina com o código de erro (ERRO_TH) usando a função "pthread_exit".

// Na função principal, as threads são criadas usando a função pthread_create.
// Em seguida, a função pthread_join é usada para esperar que as threads terminem e receber seus códigos de término.

// Isso é feito passando o endereço de um ponteiro para o código de término para "pthread_join", que atualiza o ponteiro
// com o código de término retornado pela thread.

pthread_t t1, t2;

int ERRO_TH = 111;
int SUCESSO_TH = 222;

void *codigo_tarefa_1(void){        // Utilizamos esse modelo de criação para um tipo de retorno "void*", retorna um ponteiro genérico.
    for(int ns=0; ns < 10; ++ns){
        sleep(1);
        printf("Tarefa 1: passaram %d segundos\n", ns+1);
    }
    return ( (void *) &SUCESSO_TH); // Returna o código de sucesso
}

void codigo_tarefa_2(void){
    for ( int ns = 0; ns < 15; ++ns){
        sleep(1);
        printf("Tarefa 2: passaram %d segundos\n", ns+1);
    }
    pthread_exit(&ERRO_TH); // Termina com o código de erro
}

int main(void){
    int *retorno_t1, *retorno_t2;

    printf("Inicio\n");

    pthread_create(&t1, NULL, (void *) codigo_tarefa_1, NULL);
    pthread_create(&t2, NULL, (void *) codigo_tarefa_2, NULL);

    pthread_join(t1, (void **) &retorno_t1);
    pthread_join(t2, (void **) &retorno_t2);

    printf("Fim: t1 retornou %d, t2 retornou %d\n", *retorno_t1, *retorno_t2);
}