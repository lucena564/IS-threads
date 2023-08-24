#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* 
Questão 4

Estrutura Disjoint Sets

A estrutura de dados Disjoint Sets, também conhecida como Union-Find, é uma estrutura que mantém o 
controle de uma coleção de conjuntos disjuntos. É útil para resolver problemas onde precisamos manter 
o controle de elementos agrupados.

O funcionamento:

- Find: Determina a qual conjunto um elemento pertence, ou seja, a raiz do conjunto.
- Union: Une dois conjuntos, se eles não pertencem ao mesmo conjunto.
*/

typedef struct {
    int *parent; // Armazena o índice do pai de cada elemento;
    int *rank;   // Armazena o rank (profundidade) de cada árvore;
    int n;       // Número de elementos;
} DisjointSets;

typedef struct { 
    int comeco_linha;
    int fim_linha;
    int **map;
    DisjointSets *ds;   // Ponteiro que acessará a minha struct do DisjoinSets
    int N;              // tamanho da matriz
} Param_Threads;

DisjointSets* createSets(int n);  // Cria a estrutura de conjuntos disjuntos. Inicializa cada elemento como seu próprio conjunto.
int findSet(DisjointSets *ds, int i);
void unionSets(DisjointSets *ds, int x, int y); // Une dois conjuntos, se eles não pertencem ao mesmo conjunto.

// Função para processar uma região da matriz - É a minha função das Threads.
void* processRegion(void *arg) {
    Param_Threads *args = (Param_Threads*) arg;
    int **map = args->map;
    DisjointSets *ds = args->ds;
    int N = args->N;

    for (int i = args->comeco_linha; i < args->fim_linha; i++) {
        for (int j = 0; j < N; j++) {
            if (map[i][j] == 1) {
                int index = i * N + j;
                
                // Verificar vizinhos
                int neighbors[8][2] = {
                    {i-1, j}, {i+1, j}, {i, j-1}, {i, j+1},
                    {i-1, j-1}, {i-1, j+1}, {i+1, j-1}, {i+1, j+1}
                };

                for (int k = 0; k < 8; k++) {
                    int ni = neighbors[k][0];
                    int nj = neighbors[k][1];
                    if (ni >= 0 && ni < N && nj >= 0 && nj < N && map[ni][nj] == 1) {
                        unionSets(ds, index, ni * N + nj);
                    }
                }
            }
        }
    }
    return NULL;
};

// Função principal
// Esta função é responsável por configurar e executar as threads, contar o número de ilhas e retornar esse número.
int countIslands(int **map, int N, int numThreads) {
    DisjointSets *ds = createSets(N * N);
    pthread_t threads[numThreads];
    Param_Threads args[numThreads];

    int rowsPerThread = N / numThreads;

    for (int i = 0; i < numThreads; i++) {
        args[i].comeco_linha = i * rowsPerThread;
        args[i].fim_linha = (i == numThreads - 1) ? N : (i + 1) * rowsPerThread;
        args[i].map = map;
        args[i].ds = ds;
        args[i].N = N;
        pthread_create(&threads[i], NULL, processRegion, &args[i]); // Crio as minhas threads e passo a função onde elas vão atuar 
                                                                    // junto com os argumentos.
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    int numberOfIslands = 0;
    for (int i = 0; i < N * N; i++) {
        if (ds->parent[i] == i && map[i / N][i % N] == 1) {
            numberOfIslands++;
        }
    }

    free(ds->parent);
    free(ds->rank);
    free(ds);

    return numberOfIslands;
}

DisjointSets* createSets(int n) {
    DisjointSets *ds = (DisjointSets *)malloc(sizeof(DisjointSets));
    ds->parent = (int*)malloc(n * sizeof(int));
    ds->rank = (int*)malloc(n * sizeof(int));
    ds->n = n;

    for (int i = 0; i < n; i++) {
        ds->parent[i] = i; // Cada elemento é pai de si mesmo
        ds->rank[i] = 0;   // Profundidade/rank é 0 inicialmente
    }

    return ds;
}

int findSet(DisjointSets *ds, int i) {
    if (ds->parent[i] != i) {
        ds->parent[i] = findSet(ds, ds->parent[i]); // Compressão de caminho
    }
    return ds->parent[i];
}

void unionSets(DisjointSets *ds, int x, int y) {
    int xroot = findSet(ds, x);
    int yroot = findSet(ds, y);

    // Unir as árvores pela profundidade/rank
    if (ds->rank[xroot] < ds->rank[yroot]) {
        ds->parent[xroot] = yroot;
    } else if (ds->rank[xroot] > ds->rank[yroot]) {
        ds->parent[yroot] = xroot;
    } else {
        ds->parent[yroot] = xroot;
        ds->rank[xroot]++;
    }
}

// Função para imprimir a matriz
void printMap(int **map, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", map[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    // Na quesão pede que N seja o número de threads, estou usando N como o tamanho da minha matriz pois estava
    // ficando confuso com as threads sendo N.
    int N = 5;          // Tamanho da matriz
    int numThreads = 2; // Número de threads

    // Exemplo de mapa (matriz) com 3 ilhas
    int **map = (int**) malloc(N * sizeof(int*));
    for (int i = 0; i < N; i++) {
        map[i] = (int*) malloc(N * sizeof(int));
    }

    map[0][0] = 1; map[0][1] = 1; map[0][2] = 0; map[0][3] = 0; map[0][4] = 0;
    map[1][0] = 1; map[1][1] = 1; map[1][2] = 0; map[1][3] = 1; map[1][4] = 1;
    map[2][0] = 0; map[2][1] = 0; map[2][2] = 0; map[2][3] = 1; map[2][4] = 1;
    map[3][0] = 0; map[3][1] = 1; map[3][2] = 0; map[3][3] = 0; map[3][4] = 0;
    map[4][0] = 0; map[4][1] = 1; map[4][2] = 1; map[4][3] = 0; map[4][4] = 0;

    // Imprimir a matriz
    printMap(map, N);

    int numberOfIslands = countIslands(map, N, numThreads);

    printf("Número de ilhas: %d\n", numberOfIslands);

    // Libere a memória da matriz
    for (int i = 0; i < N; i++) {
        free(map[i]);
    }
    free(map);

    return 0;
}
