package com.example;

class Util {

    public static void delay(int i) {
        try {
            Thread.sleep(1000 * i);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
    
}


class Resource {
    
    private String id;
    
    public Resource(String id) {
        this.id = id;
    }
    
    @Override
    public String toString() {
        return id;
    }
}


class ThreadA implements Runnable {
    private Object res1;
    private Object res2;

    public ThreadA(Object r1, Object r2) {
        res1 = r1;
        res2 = r2;
    }

    @Override
    public void run() {
        final String threadName = Thread.currentThread().getName();
        System.err.printf("%s running ...\n", threadName);

        System.err.printf("%s: trying to acquire %s ...", threadName, res1);
        synchronized(res1) {    // deadlock!
            System.err.printf(" acquired.\n\n");

            // Allow ThreadB to acquire res2
            Util.delay(1);

            System.err.printf("%s: trying to acquire %s ...", threadName, res2);
            synchronized(res2) {
                System.err.printf(" acquired.\n");

            }

            System.err.printf("%s: synchronized done.\n", threadName);
        }

        System.err.printf("%s done.\n", threadName);
    }
    
}


class ThreadB implements Runnable {

    private Object res1;
    private Object res2;

    public ThreadB(Object r1, Object r2) {
        res1 = r1;
        res2 = r2;
    }

    @Override
    public void run() {
        final String threadName = Thread.currentThread().getName();
        System.err.printf("%s running ...\n", threadName);

        System.err.printf("%s: trying to acquire %s ...", threadName, res2);
        synchronized(res2) {
            System.err.printf(" acquired.\n\n");

            // Allow ThreadA to acquire res1
            Util.delay(1);

            System.err.printf("%s : trying to acquire %s ...", threadName, res1);
            synchronized(res1) {    // deadlock!
                System.err.printf(" acquired.\n");

            }

            System.err.printf("%s: synchronized done.\n", threadName);
        }

        System.err.printf("%s done.\n", threadName);
    }
}



public class Synchronized {

    public static void main(String[] args) {
        new Synchronized().run();
    }
    
    
    public void run() {
        Resource r1 = new Resource("Res1");
        Resource r2 = new Resource("Res2");

        Thread t1 = new Thread(new ThreadA(r1, r2), "ThreadA");
        Thread t2 = new Thread(new ThreadB(r1, r2), "ThreadB");

        System.err.println("Starting two threads ...");
        t1.start();
        t2.start();
        System.err.println("Main done.");
    }
}
