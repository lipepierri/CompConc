import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Scanner;

class LE {
  // count é o numero de indices produzidos

  public int prod, cons, escr, lid, ord, nBlocos, count, N, in, out;
  private final Scanner scanner;
  private final PrintStream ps;
  ArrayList<ArrayList<Integer>> buffer;


  // Construtor
  LE(int N, int nBlocos, Scanner scanner, PrintStream ps) {
    this.prod = 0; // produtor produzindo (0 ou 1)
    this.cons = 0; // consumidor consumindo (0 ou 1)
    this.escr = 0; // escritor escrevendo (0 ou 1)

    this.count = 0; // Contador de inidices produzidos
    this.lid = 0; // numero de blocos lidos
    this.ord = 0; // numero de blocos ordenados

    this.nBlocos = nBlocos; // numero total de blocos;
    this.N = N; // tamanho dos blocos

    this.in = 0; // contador dos produtores
    this.out = 0; // contador dos consumidores

    this.buffer = new ArrayList<>(10);

    for (int i =0 ; i < 10; i++) {
      this.buffer.add(new ArrayList<>(N));
    }

    this.scanner = scanner;
    this.ps = ps;
  }

  // Produz
  public synchronized void produzir () {

    try {
      while (count==N) {
        wait(); //bloqueio condição Lógica
      }

      // Inserir
      // Se existir a proxima linha leia
      if (lid < nBlocos) {
        ArrayList<Integer> ac = new ArrayList<>(N);

        // Pega um bloco do texto
        for (int i = 0; i < N; i++) {
          ac.add(scanner.nextInt());
        }

        // inserir no buffer
        this.buffer.set(this.in, ac);
        lid += 1;
      }

      // Novos indice
      in=(in+1)%N; count++;


      notifyAll();
    } catch (InterruptedException ignored) { }
  }

  // Consumir
  public synchronized void consumir() {
    ArrayList<Integer> array = new ArrayList<>(this.N);

    try {
      while (this.count==0 && this.ord < this.nBlocos) {
        wait(); // bloqueio condição Lógica
      }

      if (this.ord < this.nBlocos) {
        // Pego o elemento
        array = this.buffer.get(out%N);
  
        // ordena a lista com MergeSort
        Collections.sort(array);
  
        // Altero o indice
        this.out=(this.out+1)%N; this.count--; this.ord += 1;
      }

      // Notifica todas as threads
      notifyAll();

      this.EntraEscritor();
      this.escreve(array);
      this.SaiEscritor();

    } catch(InterruptedException ignored) {
    }
  }


  // Entrada para escritores
  public synchronized void EntraEscritor() {
    try {
      if ((this.escr > 0)) {
        wait();  // bloqueia pela condicao logica da aplicacao
      }
      this.escr++; // registra que ha um escritor escrevendo
    } catch (InterruptedException ignored) { }
  }

  // Saida para escritores
  public synchronized void SaiEscritor() {
    this.escr--; //registra que o escritor saiu
    notifyAll(); //libera leitores e escritores (caso existam leitores ou escritores bloqueados)
  }

  public void escreve(ArrayList<Integer> array) {
    if (array.size() > 0) {
      StringBuilder stringBuilder = new StringBuilder();

      for (Integer i : array) {
        stringBuilder.append(" ").append(i);
      }

      String line = stringBuilder.toString();

      ps.println(line);
    }
  }
}
