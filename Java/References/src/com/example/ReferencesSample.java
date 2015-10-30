package com.example;

import java.lang.ref.PhantomReference;
import java.lang.ref.ReferenceQueue;
import java.lang.ref.SoftReference;
import java.lang.ref.WeakReference;

class Person {
    private String firstname;
    private String lastname;
    private String email;
    
    
    public Person(String first, String last, String eMail) {
        firstname = first;
        lastname = last;
        email = eMail;
    }
    
    public String toString() {
        return String.format("Person[firstname=%s, lastname=%s, email=%s]",  firstname, lastname, email);
    }
}

public class ReferencesSample {

    public void run() {
        WeakReference<Person> p1 = new WeakReference<>(new Person("Andreas", "Fester", "Andreas.Fester@example.com"));
        ReferenceQueue<Person> r1 = new ReferenceQueue<>();

        Person pers = new Person("John", "Doe", "John.Doe@example.com");
        PhantomReference<Person> p2 = new PhantomReference<>(pers, r1);

        SoftReference<Person> p3 = new SoftReference<>(new Person("Foo", "Bar", "Foo.Bar@example.com"));
        System.err.println(p1.get());
        System.err.println(p2.get());
        System.err.println(r1.poll());
        System.err.println(p3.get());
        System.err.println(pers);
    }

    public static void main(String[] args) {
        new ReferencesSample().run();
    }
}
