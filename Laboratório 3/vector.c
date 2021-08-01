#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"timer.h"

int nThreads; // num de threads
float MAXC; // variavel global que aloca o maior valor da func concorrente
float MINC; // variavel global que aloca o menor valor da func concorrente
float MAXS; // variavel global que aloca o maior valor da func sequencial
float MINS; // variavel global que aloca o menor valor da func sequencial
float *vector; // vetor principal
int *divide; // vetor de dividir o trabalho das threads

typedef struct {
  int id; // id local das threads
  long long int N; // dimensao do vetor
} tArgs;

// funcao de achar max e min concorrente
void * maxMinDetectConc(void *arg) {
  tArgs *args = (tArgs*) arg;

  int i;
  int div = args->id;
  float max = 0;
  float min = args->N + 1;

  //estou dividindo a tarefa do vetor por blocos
  for (i = divide[div]; i < divide[div+1]; i++) {
    if (vector[i] > max){
      max = vector[i];
    }
    if (vector[i] < min){
      min = vector[i];
    }
  }

  if (max > MAXC){
    MAXC=max;
  }

  if (min < MINC){
    MINC=min;
  }
  pthread_exit(NULL);
}

//versao sequencial
float sequencial(long long int tam) {
  int i;
  
  float max = 0;
  float min = tam + 1;

  //estou dividindo a tarefa do vetor por blocos
  for (i = 0; i < tam; i++) {
    if (vector[i] > max){
      max = vector[i];
    }
    if (vector[i] < min){
      min = vector[i];
    }
  }
  MAXS=max;
  MINS=min;
  return 1;
}

int main(int argc, char* argv[]) {

  long long int N; //inicializo na main para pegar com o atoll e depois passo para a struct
  double init, finish, seq, conc; //variaveis usadas pela GET_TIME()
  tArgs *args; //id e N da struct
  pthread_t *tid; //id global das threads

  //Pegando os valores passados por linha de comando, na ordem definda pelo arquivo
  if (argc<3) {
    printf("O numero de threads e o tamanho do vetor\ndevem ser lidos da linha de comando, da forma como está abaixo:\n%s Num_de_Threads Dimensao_das_Matrizes \n", argv[0]);
    return 1;
  }

  nThreads = atoi(argv[1]);
  N = atoll(argv[2]);

  if (nThreads > N) {
    printf("ERRO--nThreads precisa ser <= o tamanho do vetor.\nPasse-os por linha de comando!\n");
    return 2;
  }

  MAXC= 0;
  MINC= N+1;
  MAXS= 0;
  MINS= N+1;

  //Alocação de memória para o vetor principal e um vetor divide, criado para a divisão de tarefas em blocos das threads
  divide = malloc(sizeof(int) * (nThreads+1));
  if (divide == NULL) {
    printf("ERRO--malloc\n");
    return 2;
  }

  vector = malloc(sizeof(float) * N);
  if (vector == NULL) {
    printf("ERRO--malloc\n");
    return 2;
  }

  //Inicialização das estruturas de dados
  int i;
  for (i = 0; i < N; i++) {
    vector[i]=(((float)rand()/(float)(RAND_MAX)) * N);
  }

  //Definindo divisão em blocos para as threads
  float temp1=0;
  float temp2=0;
  for(i=1; i<(nThreads+1); i++){
    if (i==nThreads){
      divide[i-1]=temp2;
      //printf("temp2: %f\n", temp2);
      //printf("divide: %d\n", divide[i-1]);
      divide[i]= N;
      //printf("divide+1: %d\n", divide[i]);
    }
    else {
      temp1=(N/nThreads)*(i-1);
      temp2=N-temp1;
      //printf("%f\n", temp2);
      divide[i-1]=temp1;
    }
  }

  //Inicio da função concorrente
  GET_TIME(init);
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
    (args+i)->N = N;
    if (pthread_create(tid+i, NULL, maxMinDetectConc, (void*) (args+i))) {
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
  
  GET_TIME(finish);
  conc = finish - init;
  
  //função sequencial
  GET_TIME(init);

  sequencial(N);

  GET_TIME(finish);
  seq = finish - init;

  //checar se tudo deu certo
  //printf("%f, %f\n\n%f, %f\n\n", MAXC, MINC, MAXS, MINS);
  if (MAXC != MAXS || MINC != MINS) {
        puts("ERRO--Validacao");
        return 4;
    }
  printf("\nElemento MAX: %f Elemento MIN: %f\n\n", MAXC, MINC);
  free(vector);
  free(args);
  free(tid);

  printf("O concorrente levou: %e segundos\nO sequencial levou: %e segundos.\n\n", conc, seq);
  printf("Ganho de desempenho (tSeq/tConc): %e\n\n", seq/conc);

  return 0;
}
