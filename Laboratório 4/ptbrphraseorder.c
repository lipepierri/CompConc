#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 4

/* Variaveis globais */
int x = 0, y = 0;
pthread_mutex_t x_mutex;
pthread_cond_t cond1;

/* Thread 2 */
void *A (void *t) {
  printf("Seja bem-vindo!\n");
  pthread_mutex_lock(&x_mutex);
  x++;
  pthread_cond_broadcast(&cond1);
  pthread_mutex_unlock(&x_mutex);

  pthread_exit(NULL);
}

/* Thread 4 */
void *B (void *t) {
  //printf("B: Comecei\n");
  pthread_mutex_lock(&x_mutex);
  if (x == 0) {
     pthread_cond_wait(&cond1, &x_mutex);
  }
  pthread_mutex_unlock(&x_mutex);
  printf("Sente-se por favor.\n");
  pthread_mutex_lock(&x_mutex);
  x++;
  pthread_mutex_unlock(&x_mutex);
  pthread_exit(NULL);
}

/* Thread 1 */
void *C (void *t) {
  pthread_mutex_lock(&x_mutex);
  if (x == 0) {
     pthread_cond_wait(&cond1, &x_mutex);
  }
  pthread_mutex_unlock(&x_mutex);
  printf("Fique a vontade.\n");
  pthread_mutex_lock(&x_mutex);
  x++;
  pthread_mutex_unlock(&x_mutex);
  pthread_exit(NULL);
}

/* Thread 3 */
void *D (void *t) {
  while(x<3){
  } //espera as threads 1, 2 e 3 acabarem
  printf("Volte sempre!\n");
  pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[NTHREADS];

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&cond1, NULL);

  /* Cria as threads */
  pthread_create(&threads[1], NULL, C, NULL);
  pthread_create(&threads[2], NULL, A, NULL);
  pthread_create(&threads[3], NULL, D, NULL);
  pthread_create(&threads[4], NULL, B, NULL);

  /* Espera todas as threads completarem */
  for (i = 1; i < NTHREADS+1; i++) {
    pthread_join(threads[i], NULL);
  }

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&cond1);
  return 0;
}
