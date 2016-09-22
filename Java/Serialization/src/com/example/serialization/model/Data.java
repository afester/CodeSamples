package com.example.serialization.model;

import java.io.DataInputStream;
import java.io.DataOutputStream;

public interface Data {


    void encode(DataOutputStream dos);
    
    void decode(DataInputStream dis);

    DerivedDataIF createDerivedData();
//  Node createNode();
}
