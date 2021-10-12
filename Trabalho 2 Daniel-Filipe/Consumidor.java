public class Consumidor  extends Thread{
    LE le;
    int id;
    public Consumidor (int id, LE le) {
        this.id = id;
        this.le = le;
    }

    public void run() {

        while (le.ord < le.nBlocos) {
            le.consumir();
        }

//        System.out.println("thread consumidora " + this.id + " terminou de consumir");
    }
}
