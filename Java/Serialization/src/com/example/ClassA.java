package com.example;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

class ClassB implements Serializable {

    /**
     * 
     */
    private static final long serialVersionUID = -5846396277554121080L;
    int otherValue = 32;

    public ClassB(int aValue) {
        otherValue = aValue;
    }
}

class ClassC {
    private int otherValue;
    public ClassC(int aValue) {
        otherValue = aValue;
    }
}

public class ClassA extends ClassC implements Serializable {
    private static final long serialVersionUID = 2553584082189565706L;

    private int value = 42;

    // public ClassA() { super(0);}

    public ClassA(int aValue) {
        super(aValue);
        value = aValue;
    }

    public static void main(String[] args) throws FileNotFoundException, IOException, ClassNotFoundException {
        ClassA data = new ClassA(42);

        // serialize object
        try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream("object.dat"))) {
            oos.writeObject(data);
        }

        // deserialize object
        try(ObjectInputStream ois = new ObjectInputStream(new FileInputStream("object.dat"))) {
            ClassA newData = (ClassA) ois.readObject();
            System.err.println(newData.value);
        }
    }
}
