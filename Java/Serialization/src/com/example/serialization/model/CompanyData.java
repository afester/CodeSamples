package com.example.serialization.model;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.function.Function;

public class CompanyData implements Data {

    public static Function<CompanyData, DerivedDataIF> factory;

    private String name;

    // default constructor for de-serialization
    public CompanyData() {
    }

    public CompanyData(String name) {
        this.name = name;
    }


    @Override
    public final DerivedDataIF createDerivedData() {
        return factory.apply(this);
    }

    @Override
    public void encode(DataOutputStream dos) {
        try {
            dos.writeUTF(name);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void decode(DataInputStream dis) {
        try {
            name = dis.readUTF();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }


    @Override
    public String toString() {
        return String.format("CompanyData[name=%s]",  name);
    }
}
