#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

int N;
int nThreads;
int cont;
int* vector;
int* vectorSum;
pthread_mutex_t plock;
pthread_cond_t pcond;


// função concorrente
void *tarefa(void *arg) {
  int localId = (int) arg;
  int aux1 = 0;
  
  for (int i = 0; i < N; i++){
    aux1 += vector[i];
  }
  vectorSum[localId] = aux1;

  pthread_mutex_lock(&plock);
  cont++;
  if (cont < nThreads) {
    pthread_cond_wait(&pcond, &plock);
  } 
  else {
    cont = 0;
    pthread_cond_broadcast(&pcond);
  }
  pthread_mutex_unlock(&plock);

  vector[localId] = rand() % 10;

  pthread_mutex_lock(&plock);
  cont++;
  if (cont < nThreads) {
      pthread_cond_wait(&pcond, &plock);
  } 
  else {
    cont = 0;
    pthread_cond_broadcast(&pcond);
  }
  pthread_mutex_unlock(&plock);


  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  pthread_t *tid;

  pthread_mutex_init(&plock, NULL);
  pthread_cond_init(&pcond, NULL);
  
  if (argc < 2) {
    printf("Type: %s <vector dimension>\n", argv[0]);
    return 1;
  }

  N = atoll(argv[1]);
  nThreads = N;

  // Alocando memória...
  // Threads
  tid = (pthread_t *)malloc(sizeof(pthread_t) * nThreads);
	if (tid == NULL) {
		printf("ERRO--malloc\n");
		return 2;
	}

  // Vetor
  vector = malloc(N * sizeof(int *));
	if (vector == NULL) {
		printf("ERROR--malloc\n");
		return 2;
	}

  // Armazenar as somas
  vectorSum = malloc(N * sizeof(int *));
	if (vectorSum == NULL) {
		printf("ERROR--malloc\n");
		return 2;
	}

  // Inserindo números aleatórios no vetor entre 0 e 9
  for (int i = 0; i < N; i++) {
		int aux = rand() % 10;
		vector[i] = aux;
	}

  // Criação e execução das threads
  for (int i = 0; i < nThreads; i++) {
		if (pthread_create(tid + i, NULL, tarefa, (void *) i)) {
			printf("ERRO--pthread_create\n");
			return 3;
		}
	}

	for (int i = 0; i < nThreads; i++) {
		if (pthread_join(*(tid + i), NULL)) {
			printf("ERRO--pthread_join\n");
			return 3;
		}
	}

  // Função de verificação
  for (int i = 0; i < N; i++) {
     printf("Thread %d somou: %d\n", i, vectorSum[i]);
     if(vectorSum[0] != vectorSum[i]){
        printf("\nERRO--Verificacao: Thread %d somou errado!\n\n", i);
			  return 4;
     }
  }

  printf("\nO programa foi aprovado pela função de verificacao!\n\n");

  //Liberando espaço na memória
  free(vector);
  free(vectorSum);
  pthread_mutex_destroy(&plock);
  pthread_cond_destroy(&pcond);

  return 0;
}