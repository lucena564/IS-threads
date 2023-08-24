#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#define TAMANHO_DA_SENHA 10

typedef struct {
    int inicio;
    int final;
    const char* senha_correta;
    char* encontrar_senha;
} Param_Threads;

void* forca_bruta(void* arg) {
    Param_Threads* parametros = (Param_Threads*) arg;

    for (int i = parametros->inicio; i < parametros->final; i++) { 
        for (char c = 32; c < 127; c++) { // ASCII printable characters - 32 = espaço até o 126 = ~
            if (c == parametros->senha_correta[i]) {
                parametros->encontrar_senha[i] = c;
                break;
            }
        }
    }
    return NULL;
}

int main() {
    const char* senhas[] = {"abacaxi1234", "Pam0nha", "Nov9", "0123456789~@!"};
    int T_values[] = {1, 5, 10};

    for (int p = 0; p < 4; p++) {
        printf("Decifrando a senha: %s\n", senhas[p]);
        for (int t = 0; t < 3; t++) {

            int T = T_values[t];
            char encontrar_senha[TAMANHO_DA_SENHA + 1] = {0};

            pthread_t threads[T];
            Param_Threads params[T];
            
            clock_t start_time = clock();
            
            int chars_per_thread = TAMANHO_DA_SENHA / T;
            int extra_chars = TAMANHO_DA_SENHA % T;

            for (int i = 0; i < T; i++) {
                params[i].inicio = i * chars_per_thread;
                params[i].final = params[i].inicio + chars_per_thread;

                // Distribuindo os caracteres extras
                if (i < extra_chars) {
                    params[i].final += 1;
                }
                params[i].senha_correta = senhas[p];
                params[i].encontrar_senha = encontrar_senha;

                pthread_create(&threads[i], NULL, forca_bruta, &params[i]);
            }

            for (int i = 0; i < T; i++) {
                pthread_join(threads[i], NULL);
            }
            
            // clock_t execution_time = (clock() - start_time) * 1000 / CLOCKS_PER_SEC;
            clock_t execution_time = (clock() - start_time);
            
            printf("Senha encontrada com %d threads: %s. Tempo de execução: %ld milissegundos\n", T, encontrar_senha, execution_time);
        }
        printf("\n");
    }

    return 0;
}