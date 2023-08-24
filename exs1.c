#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

typedef struct {
    int inicio;
    int final;
    const char* senha_correta;
    char* encontrar_senha;
} Param_Threads;

void* forca_bruta(void* arg) {   // Vou chamar essa função para cada thread
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

    for (int p = 0; p < 4; p++) {     // Interação para pegar as senhas em senhas[] - P precisa aumentar para mais que 4 senhas.
        int tamanho_da_senha = strlen(senhas[p]); 
        printf("Decifrando a senha: %s\n", senhas[p]);

        for (int t = 0; t < 3; t++) { // Interação para a quantidade de threads, calcula para 1, 5 e 10

            int T = T_values[t];      // Variáveis para o paralelismo entre as threads.
            char encontrar_senha[tamanho_da_senha + 1];
            memset(encontrar_senha, 0, tamanho_da_senha + 1);

            pthread_t threads[T];
            Param_Threads params[T];  // Última variável necessária.

            clock_t start_time = clock(); // Starta o cronômetro da execução.

            int chars_per_thread = tamanho_da_senha / T; // Divisão de trabalho para cada thread
            int extra_chars = tamanho_da_senha % T;      // Pegando o resto da senha excedente.
            int extra_offset = 0;

            for (int i = 0; i < T; i++) { // Laço que vai criar cada thread
                params[i].inicio = i * chars_per_thread +  extra_offset; // Passando alguns parâmetros para o meu struct
                params[i].final = params[i].inicio + chars_per_thread;   // Passando alguns parâmetros para o meu struct

                // Distribuindo os caracteres extras.
                if (extra_chars > 0) {
                    params[i].final += 1;
                    extra_chars--;
                    extra_offset++;
                }

                params[i].senha_correta = senhas[p];
                params[i].encontrar_senha = encontrar_senha;

                pthread_create(&threads[i], NULL, forca_bruta, &params[i]); // Criação de cada thread e alicando sua execução na função
                                                                            // forca_bruta.
            }

            for (int i = 0; i < T; i++) { // Fechado cada thread.
                pthread_join(threads[i], NULL);
            }

            clock_t execution_time = (clock() - start_time);

            printf("Senha encontrada com %d threads: %s. Tempo de execução: %ld milissegundos\n", T, encontrar_senha, execution_time);
        }
        printf("\n");
    }

    return 0;
}
