package com.example;

class Data1 {

    private final int data;

    public Data1(int i) {
        data = i;
    }

    
    @Override
    public String toString() {
        return "Data1[data=" + data + "]";
    }
    

    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + data;
        return result;
    }


    @Override
    public boolean equals(Object obj) {
        if (this == obj)
            return true;
        if (obj == null)
            return false;
        if (getClass() != obj.getClass())
            return false;
        Data1 other = (Data1) obj;
        if (data != other.data)
            return false;
        return true;
    }
}

class Data2 {

    public String toString() {
        return "Data2";
    }

}


public class Equals {
    public static void main(String[] args) {
        new Equals().run();
    }

    private void run() {
        Object o1 = new Data1(1);
        Object o2 = new Data1(1);
        Object o3 = new Data2();
        Object o4 = new Data2();

        System.err.printf("%s equals %s ? %s\n", o1, o2, o1.equals(o2));
        System.err.printf("%s equals %s ? %s\n", o2, o1, o2.equals(o1));

        System.err.printf("%s equals %s ? %s\n", o1, o3, o1.equals(o3));
        System.err.printf("%s equals %s ? %s\n", o3, o1, o3.equals(o1));

        System.err.printf("%s equals %s ? %s\n", o3, o4, o3.equals(o4));
        System.err.printf("%s equals %s ? %s\n", o4, o3, o4.equals(o3));
    }
}
