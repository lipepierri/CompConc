#include"timer.h"
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int nThreads; // num de threads
int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499}; //vetor com os números primos
int *originalMatrix; // matriz padrao
int *copyMatrix; // armazenando os valores originais da matriz
int *divide; //vetor que armazena os pontos de divisão de blocos entre as threads 

typedef struct {
  int id; // id local das threads
  int dim; // dimensao das matrizes
} tArgs;

// funcao que irá transformar os números no primo mais próximo
void * primalize(void *arg) {
  tArgs *args = (tArgs*) arg;

  int i, j, k, temp1, temp2, temp3;
  int div = args->id;

  //estou dividindo a tarefa das matrizes por blocos
  for (i = divide[div]; i < divide[div+1]; i++) {
    temp1 = 0;
    temp2 = 0;
    temp3 = 0;
    for (j = 0; j < (args->dim); j++) {
      for(k = 0; k < 95; k++){
        if (k==0)
          temp2 = primes[k] - originalMatrix[i*(args->dim)+j]; //para não pegar um primo inexistente
        else
          temp2 = primes[k-1] - originalMatrix[i*(args->dim)+j]; //armazenando as distâncias
        temp1 = primes[k] - originalMatrix[i*(args->dim)+j]; //armazenando as distâncias
        if(temp1 >= 0){ //quando encontramos os dois primos mais próximos do valor original
            break;
          }
      }
      temp3 = originalMatrix[i*(args->dim)+j];
      if((temp2 > temp1*(-1))){ //caso o primo anterior seja mais próximo
        originalMatrix[i*(args->dim)+j] = primes[k-1];
      }
      else{ //caso o primo seguinte seja mais próximo ou tenha a mesma distância do anterior
        originalMatrix[i*(args->dim)+j] = primes[k];
      }
      if (temp3 <= 2)  //corretude no caso do valor original seja menor ou igual a 2 ou maior ou igual a 499
        originalMatrix[i*(args->dim)+j] = 2;
      if (temp3 >= 499)
        originalMatrix[i*(args->dim)+j] = 499;
    }
  }

  pthread_exit(NULL);
}

int main(int argc, char* argv[]) {

  int dim; //inicializo na main para pegar com o atoi e depois passo para a struct
  double start, end, total, totalS, dsTime, tTime, tTimeS, testTime; //variaveis usadas pela GET_TIME()
  total = 0;
  totalS = 0;
  tArgs *args; //id e dim da struct
  pthread_t *tid; //id global das threads

  //Pegando os valores passados por linha de comando, na ordem definda pelo arquivo
  GET_TIME(start);
  if (argc<3) {
    printf("O numero de threads e a dimensao das matrizes de entrada\ndevem ser lidos da linha de comando, da forma como está abaixo:\n%s Num_de_Threads Dimensao_das_Matrizes \n", argv[0]);
    return 1;
  }

  nThreads = atoi(argv[1]);
  dim = atoi(argv[2]);

  while (nThreads > dim) {
    printf("ERRO--nThreads precisa ser <= a dimensao das matrizes.\nPasse-os por linha de comando!\n");
    return 2;
  }

  //Alocação de memória para as matrizes e o vetor divide, usado para a divisão de tarefas em blocos das threads
  divide = malloc(sizeof(int) * (nThreads+1));
  if (divide == NULL) {
    printf("ERRO--malloc\n");
    return 2;
  }

  originalMatrix = malloc(sizeof(int) * dim * dim);
  if (originalMatrix == NULL) {
    printf("ERRO--malloc\n");
    return 2;
  }

  copyMatrix = malloc(sizeof(int) * dim * dim);
  if (copyMatrix == NULL) {
    printf("ERRO--malloc\n");
    return 2;
  }

  //Inicialização das estruturas de dados
  int i;
  int j;
  int k;
  for(i = 0; i < dim; i++) {
    for(j = 0; j < dim; j++) {
      originalMatrix[i*dim+j] = rand()%500+1;;
      copyMatrix[i*dim+j] = originalMatrix[i*dim+j];
    }
  }

  //Definindo divisão em blocos para as threads VERSAO CORRIGIDA
  float temp=0;
  for(i=1; i<(nThreads+1); i++){
    temp=(dim/nThreads)*(i-1);
    divide[i-1]=temp;
    divide[i]= dim;
  }

  GET_TIME(end);
  dsTime = end - start;
  total = dsTime + total;
  totalS = total;

  //Alocação de estruturas
  GET_TIME(start);
  tid = (pthread_t*) malloc(sizeof(pthread_t)*nThreads);
  if (tid==NULL) {
    puts("ERRO--malloc");
    return 2;  
  }

  args = (tArgs*) malloc(sizeof(tArgs)*nThreads);
  if (args==NULL) {
    puts("ERRO--malloc");
    return 2;  
  }

  //Criação das threads
  for(i = 0; i < nThreads; i++) {
    (args+i)->id = i;
    (args+i)->dim = dim;
    if (pthread_create(tid+i, NULL, primalize, (void*) (args+i))) {
      puts("ERRO--pthread_create");
      return 3;
    }
  }

  //Espera as threads acabarem
  for(i = 0; i < nThreads; i++) {
    if (pthread_join(*(tid+i), NULL)) {
      puts("ERRO--pthread_join");
      return 3;
    }
  }

  GET_TIME(end);
  tTime = end - start;
  tTimeS = tTime * 4;
  total = tTime + total;
  totalS = tTimeS + totalS;

  /*
  for(i = 0; i < dim; i++) {
      for(j = 0; j < dim; j++) {
        printf("| %d ", copyMatrix[i*dim+j]);
     }
     printf("|\n");
   } 
  printf("\n\n");
  for(i = 0; i < dim; i++) {
      for(j = 0; j < dim; j++) {
        printf("| %d ", originalMatrix[i*dim+j]);
     }
     printf("|\n");
   } 
   */ 
  //^ caso queira imprimir a matriz com os valores antigos e novos, remova os espaços comentados (/* e */)

  // Verificação se o resultado está de acordo com o esperado
  GET_TIME(start);
  for(i = 0; i < dim; i++) {
     for(j = 0; j < dim; j++) {
       for(k = 0; k < 96; ){
         while (originalMatrix[i*dim+j] != primes[k]) { //checando se ele é primo
           k++;
           if (k==95){
            puts("ERRO--Validacao NOT PRIMO");
            return 4;
            }
         }
         int testa = copyMatrix[i*dim+j] - primes[k-1]; //checando se ele é o primo mais próximo
         int testab = primes[k] - copyMatrix[i*dim+j];
         int testb = primes[k+1] - copyMatrix[i*dim+j];
         if ((testa < testab) || (testa < (testab*(-1))) || (testb < testab) || (testb < (testab*(-1)))){
           if (primes[k-1] < 2 || primes[k+1] < 2) //caso ele pegue um ponto que não faça parte do vetor
            break;
           printf("\ntermo original=%d, termo novo=%d\nprimo anterior=%d, primo posterior=%d\n\n", copyMatrix[i*dim+j], originalMatrix[i*dim+j], primes[k-1], primes[k+1]);
           puts("ERRO--Validacao NOT MAIS PROX");
           return 4;
         }
         k=96;
      }
    }
  }
  

  free(originalMatrix);
  free(copyMatrix);
  free(args);
  free(tid);

  printf("\nAs funcoes de verificacao indicaram que o programa está funcionando da forma adequada!\n\n");

  GET_TIME(end);
  testTime = (end - start) + total;
  total = testTime + total;
  totalS = testTime + totalS;

  printf("\nInicializacao das estruturas de dados levou %e segundos.\n\n", dsTime);
  printf("Criacao das threads, execucao da funcao, e termino das threads levou %e segundos.\n\n", tTime);
  printf("Esse mesmo trabalho das threads, se feito de forma sequencial, teria levado %e segundos na máquina de Filipe.\n\n", tTimeS);
  printf("A execução das funções de verificação levou %e segundos.\n\n", testTime);
  printf("Tempo ate a finalizacao do programa levou %e segundos.\n\n", total);
  printf("Esse mesmo trabalho, se feito de forma sequencial, teria levado %e segundos para o encerramento do programa na máquina de Filipe.\n\n", totalS);
  printf("\nGanho de desempenho: %e \n\n", totalS/total);

  return 0;
}