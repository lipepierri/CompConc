#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define NTHREADSCONS 4
#define NTHREADSPROD 4
#define N 100

/* Variaveis globais */
int x = 0;
int *buffer;
sem_t consGo, prodGo;

/* F Produtora */
void *P (void *t) {
  while (1) {
  sem_wait(&prodGo);
  if (x>=(N-1)){
    printf("Produtor terminou seu serviço\n\n\n\n");
    
    sem_post(&consGo);
    sem_wait(&prodGo);
  }
  printf("Produzindo... %d\n", x);
  buffer[x]=1;
  
  x++;
  sem_post(&prodGo);
  }
  pthread_exit(NULL);
}

/* F Consumidora */
void *C (void *t) {
  while (1) {
  sem_wait(&consGo);
  if (x<=0){
    printf("Consumidor terminou seu serviço\n\n\n\n");
    
    sem_post(&prodGo);
    sem_wait(&consGo);
  }
  printf("Consumindo... %d\n", x);
  buffer[x]=NULL;
  x--;
  sem_post(&consGo);
  }
  pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  int i, j, k; 
  pthread_t threads[NTHREADSCONS];

  //Alocação de memória para o buffer
  buffer = malloc(sizeof(int) * (N));
  if (buffer == NULL) {
    printf("ERRO--malloc\n");
    return 2;
  }

  /* Inicilaiza o semaforo */
  sem_init(&prodGo, 0, 0);
  sem_init(&consGo, 0, 0);
  sem_post(&prodGo);

  /* Cria as threads */
  for (i = 0; i < NTHREADSPROD; i++) {
    pthread_create(&threads[i], NULL, P, NULL);
  }
  for (j = NTHREADSPROD; j < (NTHREADSCONS+NTHREADSPROD); j++) {
    pthread_create(&threads[j], NULL, C, NULL);
  }

  
  /* Espera todas as threads completarem */
  for (k = 0; k < (NTHREADSCONS+NTHREADSPROD); k++) {
    pthread_join(threads[k], NULL);
  }

  return 0;
}
