/* Disciplina: Computacao Concorrente */
/* Professora: Silvana Rossetto */
/* Aluno: Filipe Nobre | DRE: 118033026 */
/* Módulo 1 - Laboratório: 1 - Atividade 5 */
/* Codigo: "Implementar um programa concorrente, com duas threads (alem da thread principal), 
para elevar ao quadrado cada elemento de um vetor de 10000 elementos. */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS 2 //total de threads a serem criadas
#define TAM 10000
int vector[TAM];

//funcao executada pelas threads
void * Square (void *arg) {
  int init = * (int *) arg;
  int cont;
  //para não repetir a posição, elas calculam de dois em dois elementos do vetor
  for(cont=init; cont<TAM; cont+=2){
    vector[cont]=vector[cont]*vector[cont];
    }
  
  pthread_exit(NULL);
}

//funcao principal do programa
int main() {
  pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
  int thread; //variável auxiliar
  int copyvector[TAM]; //vetor copia que contera os valores do vetor original antes de altera-los
  int init[2]={0,1}; //define a posição inicial no vetor que cada thread irá começar a trabalhar
  int i; //variavel auxiliar

  for(i=0;i<TAM;i++)
    {
        vector[i]=rand()%TAM+1; //preenchendo o vetor com valores aleatórios
        copyvector[i]=vector[i]; //copiando o vetor original para podermos comparar os resultados depois
    }

  //cria thread 1
  if (pthread_create(&tid_sistema[0], NULL, Square, &init[0])) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  //cria thread 2
  if (pthread_create(&tid_sistema[1], NULL, Square, &init[1])) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  
  //--espera todas as threads terminarem
  for (thread=0; thread<NTHREADS; thread++) {
    if (pthread_join(tid_sistema[thread], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
    } 
  }

  //como pedido no enunciado, verifico se os valores finais do vetor estao corretos dentro da main
  for (i=0; i<TAM; i++){
    if (copyvector[i] != (vector[i])/(copyvector[i])) {
        printf("--ERRO: Algo deu errado na hora de calcular!\n"); exit(-1);
    }
  }

  printf("--Thread principal terminou da maneira esperada\n");
  pthread_exit(NULL);
}
