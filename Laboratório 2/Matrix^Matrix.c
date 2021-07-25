#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"timer.h"

int nThreads; // num de threads
float *firstMatrix; // primeira matriz
float *secondMatrix; // segunda matriz
float *resultMatrix; // resultado da multiplcacao
int *divide;

typedef struct {
  int id; // id local das threads
  int dim; // dimensao das matrizes
} tArgs;

// funcao de multiplicar
void * multiplying(void *arg) {
  tArgs *args = (tArgs*) arg;

  int i,j,k;
  int div = args->id;
  float temp;

  //estou dividindo a tarefa das matrizes por blocos
  for (i = divide[div]; i < divide[div+1]; i++) {
    for (j = 0; j < (args->dim); j++) {
      temp = 0;
      for(k = 0; k < (args->dim); k++) {
        temp += firstMatrix[j*(args->dim)+k] * secondMatrix[k*(args->dim)+i];
      }
      resultMatrix[j * (args->dim) + i] = temp;
    }
  }

  pthread_exit(NULL);
}

int main(int argc, char* argv[]) {

  int dim; //inicializo na main para pegar com o atoi e depois passo para a struct
  double init, finish, total, initDS, endOfThreads; //variaveis usadas pela GET_TIME()
  total=0;
  tArgs *args; //id e dim da struct
  pthread_t *tid; //id global das threads

  //Pegando os valores passados por linha de comando, na ordem definda pelo arquivo
  GET_TIME(init);
  if (argc<3) {
    printf("O numero de threads e a dimensao das matrizes de entrada\ndevem ser lidos da linha de comando, da forma como está abaixo:\n%s Num_de_Threads Dimensao_das_Matrizes \n", argv[0]);
    return 1;
  }

  nThreads = atoi(argv[1]);
  dim = atoi(argv[2]);

  while (nThreads > dim) {
    printf("ERROR--nThreads precisa ser <= a dimensao das matrizes.\nPasse-os por linha de comando!\n");
    return 2;
  }

  //Alocação de memória para as matrizes e o vetor divide, usado para a divisão de tarefas em blocos das threads
  divide = malloc(sizeof(int) * (nThreads+1));
  if (divide == NULL) {
    printf("ERRO--malloc\n");
    return 2;
  }

  firstMatrix = malloc(sizeof(float) * dim * dim);
  if (firstMatrix == NULL) {
    printf("ERRO--malloc\n");
    return 2;
  }

  secondMatrix = malloc(sizeof(float) * dim * dim);
  if (secondMatrix == NULL) {
    printf("ERROR--malloc\n");
    return 2;
  }

  resultMatrix = malloc(sizeof(float) * dim * dim);
  if (resultMatrix == NULL) {
    printf("ERROR--malloc\n");
    return 2;
  }

  //Inicialização das estruturas de dados
  int i;
  int j;
  for(i = 0; i < dim; i++) {
    for(j = 0; j < dim; j++) {
      firstMatrix[i*dim+j] = 2; //estou usando 2 para facilitar a função verificação
      secondMatrix[i*dim+j] = 2;
      resultMatrix[i*dim+j] = 0;
    }
  }

  //Definindo divisão em blocos para as threads
  float temp1=0;
  float temp2=0;
  for(i=1; i<(nThreads+1); i++){
    if (i==nThreads){
      divide[i-1]=temp2;
      //printf("temp2: %f\n", temp2);
      //printf("divide: %d\n", divide[i-1]);
      divide[i]= dim;
      //printf("divide+1: %d\n", divide[i]);
    }
    else {
      temp1=(dim/nThreads)*(i-1);
      temp2=dim-temp1;
      //printf("%f\n", temp2);
      divide[i-1]=temp1;
    }
  }

  GET_TIME(finish);
  initDS = finish - init;
  total = initDS + total;

  //Multiplicação das matrizes e alocação de estruturas
  GET_TIME(init);
  tid = (pthread_t*) malloc(sizeof(pthread_t)*nThreads);
  if (tid==NULL) {
    puts("ERROR--malloc");
    return 2;  
  }

  args = (tArgs*) malloc(sizeof(tArgs)*nThreads);
  if (args==NULL) {
    puts("ERROR--malloc");
    return 2;  
  }

  //Criação das threads
  for(i = 0; i < nThreads; i++) {
    (args+i)->id = i;
    (args+i)->dim = dim;
    if (pthread_create(tid+i, NULL, multiplying, (void*) (args+i))) {
      puts("ERROR--pthread_create");
      return 3;
    }
  }

  //Espera as threads acabarem
  for(i = 0; i < nThreads; i++) {
    if (pthread_join(*(tid+i), NULL)) {
      puts("ERROR--pthread_join");
      return 3;
    }
  }
  
  GET_TIME(finish);
  endOfThreads = finish - init;
  total = endOfThreads + total;
  
  // for(i = 0; i < dim; i++) {
  //    for(j = 0; j < dim; j++) {
  //     printf("%f ", resultMatrix[i*dim+j]);
  //   } printf("\n");
  // }   --estava printando a matriz que armazena o resultado para testes 

  // Verificação se o resultado está de acordo com o esperado
  GET_TIME(init);
  for(i = 0; i < dim; i++) {
     for(j = 0; j < dim; j++) {
      if (resultMatrix[i*dim+j] != 4*dim) {
        puts("ERROR--Validacao");
        return 4;
      };
    }
  }

  free(firstMatrix);
  free(secondMatrix);
  free(resultMatrix);
  free(args);
  free(tid);

  GET_TIME(finish);
  total = (finish - init) + total;

  printf("\nInicializacao das estruturas de dados levou %e segundos.\n\n", initDS);
  printf("Criacao das threads, execucao da multiplicacao, e termino das threads levou %e segundos.\n\n", endOfThreads);
  printf("Tempo ate a finalizacao do programa levou %e segundos\n\n", total);

  return 0;
}