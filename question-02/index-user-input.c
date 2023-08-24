/**
1. Definição do algoritmo:
- Registra alocação de recursos e verifica se há dependências circulares fazendo
o uso da regra busca em profundidade (DFS), percorrendo o grafo, marcando os
processos e recursos visitados. Em resumo, se durante a busca for encontrado um
nó repetido existe uma espera circular e, consequentemente, um Deadlock.
**/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Constantes globais que representam, respectivamente, os números de
// recursos e processos para nossa demonstração
#define NUMBER_RESOURCES 3
#define NUMBER_PROCESSES 3

// Definindo um enum Boolean com constantes para representar valores booleanos,
// já que a linguagem C não tem de forma nativa, para facilitar a legibilidade
// do código
enum Boolean { False, True };
typedef enum Boolean bool;

bool resource_status[NUMBER_RESOURCES] = {False, False,
                                          False}; // False: livre, True: alocado

// Estrutura de Nó
typedef struct Node {
  int id;
  bool visited;
  bool in_stack;
  struct Node *next;
} Node;

// Estrutura de processo
typedef struct {
  int id;
  int allocated_resources[NUMBER_RESOURCES];
} Process;

Process processes[NUMBER_PROCESSES];

pthread_mutex_t resource_mutex[NUMBER_RESOURCES];

// Grafo representado por uma lista de adjacências
Node *graph[NUMBER_PROCESSES];

// Cria um novo nó do grafo
Node *create_node(int id) {
  Node *node = (Node *)malloc(sizeof(Node));
  node->id = id;
  node->visited = False;
  node->in_stack = False;
  node->next = NULL;
  return node;
}

// Adiciona uma aresta (aresta direcionada) da origem para o destino
void add_edge(Node *source, Node *destination) {
  Node *newNode = create_node(destination->id);
  newNode->next = source->next;
  source->next = newNode;
}

// Verifica se há um ciclo no grafo a partir do nó dado
bool has_cycle(Node *current_process) {
  if (!current_process->visited) {
    current_process->visited = True;  // Marca o nó como visitado
    current_process->in_stack = True; // Marca o nó como parte da pilha de busca
                                      // (para detecção de ciclo)

    Node *adjacent_process =
        graph[current_process->id]->next; // Pega o primeiro nó adjacente

    while (adjacent_process != NULL) {
      if (!adjacent_process->visited && has_cycle(adjacent_process)) {
        // Se o nó adjacente não foi visitado e há um ciclo no seu subgrafo, há
        // um ciclo no total
        return True;
      } else if (adjacent_process->in_stack) {
        // Se o nó adjacente já está na pilha de busca, significa que há um
        // ciclo
        return True;
      }
      adjacent_process =
          adjacent_process->next; // Avança para o próximo nó adjacente
    }

    current_process->in_stack =
        False; // Remove o nó da pilha de busca ao sair da função recursiva
  }
  return False; // Se não há ciclo no subgrafo a partir deste nó, retorna False
}

// Função executada por cada thread para verificar deadlock
void *check_deadlock(void *arg) {
  Process *process = (Process *)arg;
  Node *current_process = graph[process->id];

  if (has_cycle(current_process)) {
    printf("  Deadlock detected for process %d\n", process->id);
  } else {
    printf("  No deadlock detected for process %d\n", process->id);
  }

  return NULL;
}

int main() {
  pthread_t threads[NUMBER_PROCESSES];

  // Inicialização dos mutexes
  for (int i = 0; i < NUMBER_RESOURCES; ++i) {
    pthread_mutex_init(&resource_mutex[i], NULL);
  }

  printf("\nLike a computer, input this information for 3 processes:\n");
  // Definição dos processos e recursos
  for (int i = 0; i < NUMBER_PROCESSES; ++i) {
    processes[i].id = i;

    printf("\n%d. Enter allocated resources for process %d:\n", i + 1, i);
    for (int j = 0; j < NUMBER_RESOURCES; ++j) {
      int allocated;
      printf("  - Resource %d (0: Not Allocated, 1: Allocated): ", j);
      scanf("%d", &allocated);
      processes[i].allocated_resources[j] = (allocated == 1) ? True : False;
    }
  }

  // Inicialização dos nós do grafo
  for (int i = 0; i < NUMBER_PROCESSES; ++i) {
    graph[i] = create_node(i);
  }

  // Criando as arestas do grafo com base nas alocações de recursos
  for (int i = 0; i < NUMBER_PROCESSES; ++i) {
    for (int j = 0; j < NUMBER_RESOURCES; ++j) {
      if (processes[i].allocated_resources[j]) {
        add_edge(graph[i], graph[j]);
      }
    }
  }

  // Inicialização das estruturas de dados para busca em profundidade
  for (int i = 0; i < NUMBER_PROCESSES; ++i) {
    graph[i]->visited = False;
    graph[i]->in_stack = False;
  }

  printf("\nDetection results:\n");
  // Criação das threads
  for (int i = 0; i < NUMBER_PROCESSES; ++i) {
    pthread_create(&threads[i], NULL, check_deadlock, &processes[i]);
  }

  // Espera pelas threads terminarem
  for (int i = 0; i < NUMBER_PROCESSES; ++i) {
    pthread_join(threads[i], NULL);
  }

  // Liberação dos mutexes
  for (int i = 0; i < NUMBER_RESOURCES; ++i) {
    pthread_mutex_destroy(&resource_mutex[i]);
  }

  // Liberação da memória dos nós do grafo
  for (int i = 0; i < NUMBER_PROCESSES; ++i) {
    Node *current = graph[i];
    while (current != NULL) {
      Node *temp = current;
      current = current->next;
      free(temp);
    }
  }

  return 0;
}
