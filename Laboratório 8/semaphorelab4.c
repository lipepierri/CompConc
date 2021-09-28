#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 4

/* Variaveis globais */
int x = 0;
sem_t em, step;

/* Thread 2 */
void *A (void *t) {
  printf("Seja bem-vindo!\n");
  x=1;
  sem_post(&em);
  pthread_exit(NULL);
}

/* Thread 4 */
void *B (void *t) {
  sem_wait(&em);
  printf("Sente-se por favor.\n");
  x++;
  sem_post(&em);
  if (x==3){
    sem_post(&step);
  }
  pthread_exit(NULL);
}

/* Thread 1 */
void *C (void *t) {
  sem_wait(&em);
  printf("Fique a vontade.\n");
  x++;
  sem_post(&em);
  if (x==3){
    sem_post(&step);
  }
  pthread_exit(NULL);
}

/* Thread 3 */
void *D (void *t) {
  sem_wait(&step);
  printf("Volte sempre!\n");
  pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[NTHREADS];

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  sem_init(&em, 0, 0);
  sem_init(&step, 0, 0);

  /* Cria as threads */
  pthread_create(&threads[1], NULL, C, NULL);
  pthread_create(&threads[2], NULL, A, NULL);
  pthread_create(&threads[3], NULL, D, NULL);
  pthread_create(&threads[4], NULL, B, NULL);

  /* Espera todas as threads completarem */
  for (i = 1; i < NTHREADS+1; i++) {
    pthread_join(threads[i], NULL);
  }

  return 0;
}
