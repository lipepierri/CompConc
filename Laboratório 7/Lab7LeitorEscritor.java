/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Leitores e escritores usando monitores em Java */
/* -------------------------------------------------------------------*/

// Monitor que implementa a logica do padrao leitores/escritores
class LE {
  private int leit, escr, leitescr;
  public static int variavel = 0;

  // Construtor
  LE() { 
     this.leit = 0; //leitores lendo (0 ou mais)
     this.escr = 0; //escritor escrevendo (0 ou 1)
     this.leitescr = 0; //leitores lendo (0 ou mais)
  } 
  
  // Entrada para leitores
  public synchronized void EntraLeitor (int id) {
    try { 
      //while (this.escr > 0) {
      if ((this.escr > 0) || (this.leitescr > 0)) {
         //System.out.println ("le.leitorBloqueado("+id+")");
         wait();  //bloqueia pela condicao logica da aplicacao 
      }
      this.leit++;  //registra que ha mais um leitor lendo
      //System.out.println ("le.leitorLendo("+id+")");
    } catch (InterruptedException e) { }
  }
  
  // Saida para leitores
  public synchronized void SaiLeitor (int id) {
     this.leit--; //registra que um leitor saiu
     if (this.leit == 0) 
           this.notify(); //libera escritor (caso exista escritor bloqueado)
     //System.out.println ("le.leitorSaindo("+id+")");
  }
  
  // Entrada para escritores
  public synchronized void EntraEscritor (int id) {
    try { 
      //while ((this.leit > 0) || (this.escr > 0)) {
      if ((this.leit > 0) || (this.escr > 0) || (this.leitescr > 0)) {
         //System.out.println ("le.escritorBloqueado("+id+")");
         wait();  //bloqueia pela condicao logica da aplicacao 
      }
      this.escr++; //registra que ha um escritor escrevendo
      //System.out.println ("le.escritorEscrevendo("+id+")");
    } catch (InterruptedException e) { }
  }
  
  // Saida para escritores
  public synchronized void SaiEscritor (int id) {
     this.escr--; //registra que o escritor saiu
     notifyAll(); //libera leitores e escritores (caso existam leitores ou escritores bloqueados)
     //System.out.println ("le.escritorSaindo("+id+")");
  }

  // Entrada para leitores-escritores
  public synchronized void EntraLeitorEscritor (int id) {
    try {
      //while ((this.leit > 0) || (this.escr > 0)) {
      if ((this.leit > 0) || (this.escr > 0) || (this.leitescr > 0)) {
        //System.out.println ("le.leitorescritorBloqueado("+id+")");
        wait();  //bloqueia pela condicao logica da aplicacao
      }
      this.leitescr++; //registra que ha um leitor-escritor lendo e escrevendo
      //System.out.println ("le.leitorescritorLendo-Escrevendo("+id+")");
    } catch (InterruptedException e) { }
  }

  // Saida para leitores-escritores
  public synchronized void SaiLeitorEscritor (int id) {
    this.leitescr--; //registra que o leitor-escritor saiu
    notifyAll(); //libera leitores e escritores (caso existam leitores ou escritores bloqueados)
    //System.out.println ("le.leitorescritorSaindo("+id+")");
  }

  public static synchronized void fL() {
    //ver se é primo
    boolean flag = false;
    for (int i = 2; i <= (variavel / 2); ++i) {
      if (variavel % i == 0) { //condição para o numero nao ser primo
        flag = true;
        break;
      }
    }
    if (!flag && (variavel >= 2))
      System.out.println(variavel + " eh um numero primo.");
    else
      System.out.println(variavel + " nao eh um numero primo.");
  }


  public static synchronized void fE(int id) {
    //escrever id na variável
    variavel = id;
  }

  public static synchronized void fLE() {
    //verifica se é par ou ímpar
    if(variavel % 2 == 0)
      System.out.println(variavel + " eh um numero par.");
    else
      System.out.println(variavel + " eh um numero impar.");

    //multiplica a variavel por 2
    variavel = (variavel*2);
  }

}



//Aplicacao de exemplo--------------------------------------------------------
// Leitor
class Leitor extends Thread {
  int id; //identificador da thread
  int delay; //atraso bobo
  LE monitor;//objeto monitor para coordenar a lógica de execução das threads

  // Construtor
  Leitor (int id, int delayTime, LE m) {
    this.id = id;
    this.delay = delayTime;
    this.monitor = m;
  }

  // Método executado pela thread
  public void run () {
    try {
      for (;;) {
        this.monitor.EntraLeitor(this.id);

        monitor.fL();

        this.monitor.SaiLeitor(this.id);
        sleep(this.delay); 
      }
    } catch (InterruptedException e) { return; }
  }
}

//--------------------------------------------------------
// Escritor
class Escritor extends Thread {
  int id; //identificador da thread
  int delay; //atraso bobo...
  LE monitor; //objeto monitor para coordenar a lógica de execução das threads

  // Construtor
  Escritor (int id, int delayTime, LE m) {
    this.id = id;
    this.delay = delayTime;
    this.monitor = m;
  }

  // Método executado pela thread
  public void run () {
    try {
      for (;;) {
        this.monitor.EntraEscritor(this.id);

        monitor.fE(this.id);

        this.monitor.SaiEscritor(this.id); 
        sleep(this.delay);
      }
    } catch (InterruptedException e) { return; }
  }
}
//------------------------------------------------------------------------------------------------------------
// LeitorEscritor
class LeitorEscritor extends Thread {
  int id; //identificador da thread
  int delay; //atraso bobo
  LE monitor;//objeto monitor para coordenar a lógica de execução das threads

  // Construtor
  LeitorEscritor (int id, int delayTime, LE m) {
    this.id = id;
    this.delay = delayTime;
    this.monitor = m;
  }

  // Método executado pela thread
  public void run () {
    try {
      for (;;) {
        this.monitor.EntraLeitorEscritor(this.id);

        monitor.fLE();

        this.monitor.SaiLeitorEscritor(this.id);
        sleep(this.delay);
      }
    } catch (InterruptedException e) { return; }
  }
}

//--------------------------------------------------------
// Classe principal
class Lab7LeitorEscritor {
  static final int L = 5;
  static final int E = 5;
  static final int LE = 5;

  public static void main (String[] args) {
    int i;
    LE monitor = new LE();                  // Monitor (objeto compartilhado entre leitores e escritores)
    Leitor[] l = new Leitor[L];             // Threads leitores
    Escritor[] e = new Escritor[E];         // Threads escritores
    LeitorEscritor[] le = new LeitorEscritor[LE]; // Threads leitor-escritores

    //inicia o log de saida
    //System.out.println ("import verificaLE");
    //System.out.println ("le = verificaLE.LE()");
    
    for (i=0; i<L; i++) {
       l[i] = new Leitor(i+1, (i+1)*500, monitor);
       l[i].start(); 
    }
    for (i=0; i<E; i++) {
       e[i] = new Escritor(i+1, (i+1)*500, monitor);
       e[i].start();
    }
    for (i=0; i<LE; i++) {
       le[i] = new LeitorEscritor(i+1, (i+1)*500, monitor);
       le[i].start();
    }
  }
}
