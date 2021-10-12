public class Produtor extends Thread  {
    LE le;
    int id;

    public Produtor (int id, LE le) {
        this.id = id;
        this.le = le;
    }

    public void run() {
        while (le.lid < le.nBlocos) {
            le.produzir();
        }

//        System.out.println("Terminei de produzir");

    }
}