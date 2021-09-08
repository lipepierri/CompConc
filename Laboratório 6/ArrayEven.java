import java.util.Scanner;
import java.util.Random;
import java.util.Arrays;

class ArrayCompartilhado {
   private int[] array;
   public static int quantEven = 0;

   // Inicializa o array com valores aleatorios
   public ArrayCompartilhado(int length, int max) {
      this.array = new int[length];
      Random rnd = new Random();
      for (int i = 0; i < length; i++ ) {
         this.array[i] = rnd.nextInt(max*2 - 1) + 1;
      }
   }

   public synchronized int[] get() {
      return this.array;
   }

   public static synchronized void evenSum() {
      quantEven++;
   }
}

// Funcao da Threads
class T extends Thread {
   private int id;

   int[] array;

   int NThreads;

   int quantEven = 0;

   public T(int tid, ArrayCompartilhado a, int n) {
      this.id = tid;
      this.array = a.get();
      this.NThreads = n;
   }

   public void run() {
      for (int i = id; i < array.length; i += NThreads) {
         if(array[i]%2 == 0){
            ArrayCompartilhado.evenSum();
         }
      }
   }
}

// Funcao main
public class ArrayEven {
   public static void main (String[] args) {
      Scanner sc = new Scanner(System.in);

      System.out.println("Entre com o tamanho do vetor:");
      int DIM = sc.nextInt();
      System.out.println("Entre com o numero de Threads:");
      int NThreads = sc.nextInt();

      sc.close();

      if (NThreads > DIM) {
         NThreads = DIM;
      }

      // Cria o array de threads
      Thread[] threads = new Thread[NThreads];

      // Cria uma instância de um array que será compartilhada pelas Threadds
      ArrayCompartilhado array = new ArrayCompartilhado(DIM, NThreads);

      // Cria as Threads
      for (int i=0; i < threads.length; i++) {
         threads[i] = new T(i, array, NThreads);
      }

      // Inicia as Threads
      for (int i=0; i<threads.length; i++) {
         threads[i].start();
      }

      // Esperar fim das Threads
      for (int i=0; i < threads.length; i++) {
         try {
            threads[i].join();
         } catch (InterruptedException e) {
            return;
         }
      }

      // Funcao de verificacao
      int validacao = 0;
      for (int element: array.get()) {
         if (element%2 == 0) {
            validacao++;
         }
      }
      if(validacao != ArrayCompartilhado.quantEven){
         System.out.println("--Erro de validação");
         return;
      }

      System.out.println("Passou na validacao");
      //System.out.println(Arrays.toString(array.get()));
   }

}