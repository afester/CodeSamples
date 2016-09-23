package com.example.serialization;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.List;

import com.example.hexdump.HexDump;
import com.example.serialization.model.CompanyData;
import com.example.serialization.model.Data;
import com.example.serialization.model.DerivedDataIF;
import com.example.serialization.model.PersonData;

public class Example {

    public static void main(String[] args) {
        try {
            new Example().run();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (SecurityException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        } catch (InstantiationException e) {
            e.printStackTrace();
        }
    }
    
    private DerivedData factoryForPerson(PersonData p) {
        return new DerivedData("Person: " + p);
    }

    private DerivedData factoryForCompany(CompanyData c) {
        return new DerivedData("Company: " + c);
    }


    private void run() throws IOException, ClassNotFoundException, NoSuchMethodException, SecurityException,
            IllegalAccessException, IllegalArgumentException, InvocationTargetException, InstantiationException {

        // inject factories for derived data into the model layer
        PersonData.factory = this::factoryForPerson;
        CompanyData.factory = this::factoryForCompany;

        // Beware: writeUTF8() only supports Strings up to 64k
        // StringBuffer longString = new StringBuffer("Hello World");
        // for (int i = 0; i < 6554; i++) {
        // longString.append("Hello Moon");
        // }
        // System.err.println("LENGTH:" + longString.length());

        // create sample data
        List<Data> sequence = new ArrayList<>();
        sequence.add(new PersonData("Andreas", "Fester"));
        sequence.add(new CompanyData("Oracle"));
        sequence.add(new PersonData("Fritz", "Ferkel"));

        // serialize the list of sample data
        byte[] result = encode(sequence);

        // dump the resulting byte array
        HexDump hd = new HexDump(result);
        hd.dumpAll(System.err);

        // deserialize the list
        List<Data> recreatedList = decode(result);
        System.err.println(recreatedList);

        // for each recreated list element, call the factory method to create derived data
        for (Data d : recreatedList) {
            DerivedDataIF dr = d.createDerivedData();
            System.err.println(dr);
        }
    }

    private List<Data> decode(byte[] data)
            throws IOException, ClassNotFoundException, NoSuchMethodException, SecurityException,
            IllegalAccessException, IllegalArgumentException, InvocationTargetException, InstantiationException {

        ByteArrayInputStream bais = new ByteArrayInputStream(data);
        DataInputStream dis = new DataInputStream(bais);

        List<Data> sequence = new ArrayList<>();

        while (dis.available() > 0) {
            String segmentType = dis.readUTF();
            Class<?> clazz = Class.forName(segmentType);

//            // Approach 1: call static method
//            Method decode = clazz.getMethod("decode", DataInputStream.class);
//            Data d = (Data) decode.invoke(null, dis);
//            System.err.println(d);

            // Approach 2: Create object and call member method (requires
            // default constructor!)
            Data d = (Data) clazz.newInstance();
            d.decode(dis);
            sequence.add(d);
        }

        return sequence;
    }

    private byte[] encode(List<Data> sequence) throws IOException {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(baos);
        for (Data d : sequence) {
            dos.writeUTF(d.getClass().getName());
            d.encode(dos);
        }
        dos.close();

        return baos.toByteArray();
    }
}
