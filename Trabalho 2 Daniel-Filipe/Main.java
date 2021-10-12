import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintStream;
import java.util.Date;
import java.util.Scanner;

class Main {

  public static void main (String[] args) {

    long inicio = new Date().getTime();

    int i, N, nElements, nBlocos, C;
    String input, output;
    Scanner scanner = null;
    PrintStream ps = null;

    // Verifica se o número de argumentos é igual a 4
    if (args.length != 4) {
      System.out.println("Digite: java Main <nThreadConsumidoras> <DimensãoDoBloco> <nomeArquivoEntrada> <nomeArquivoSaida>");
      System.exit(-1);
    }

    // Captura argumentos em linha de comando
    C = Integer.parseInt(args[0]);
    N = Integer.parseInt(args[1]);
    input = args[2];
    output = args[3];

    try {
      scanner = new Scanner(new File (input));
      ps = new PrintStream(output);
    } catch (FileNotFoundException e) {
      System.out.println("Arquivo não encontrado");
      System.exit(-1);
    }

    nElements = scanner.nextInt();
    nBlocos = nElements / N;

    LE monitor = new LE(N, nBlocos, scanner, ps);
    Consumidor[] c = new Consumidor[C];

    // Iniciar a produtora
    Produtor p = new Produtor(1, monitor);
    p.start();

    //  Iniciar as consumidoras
    long concorrente = new Date().getTime();

    for (i=0; i<C; i++) {
      c[i] = new Consumidor(i+1, monitor);
      c[i].start();
    }

    try {
      for (i=0; i<C; i++) {
        c[i].join();
      }
    } catch (Exception ignored) {}

    long fim = new Date().getTime();

    double calcSequencial = (concorrente - inicio) + (fim - concorrente)*4;
    double calcConcorrente = (concorrente - inicio) + (fim - concorrente);
    double calcDesempenho = ((calcSequencial / calcConcorrente));

    System.out.println("Ganho de desempenho: "+ calcDesempenho);

  }
}