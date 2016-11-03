package com.example.serialization.model;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.function.Function;

public class PersonData implements Data {

    public static Function<PersonData, DerivedDataIF> factory;

    private String firstName;
    private String lastName;

    // default constructor for de-serialization
    public PersonData() {
    }

    @Override
    public final DerivedDataIF createDerivedData() {
        return factory.apply(this);
    }

    public PersonData(String first, String last) {
        this.firstName = first;
        this.lastName = last;
    }

    @Override
    public void encode(DataOutputStream dos) {
        try {
            dos.writeUTF(firstName);
            dos.writeUTF(lastName);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void decode(DataInputStream dis) {
        try {
            firstName = dis.readUTF();
            lastName = dis.readUTF();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public String toString() {
        return String.format("PersonData[firstName=%s, lastName=%s]",  firstName, lastName);
    }
}
