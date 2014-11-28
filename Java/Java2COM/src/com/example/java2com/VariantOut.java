package com.example.java2com;


public class VariantOut extends Variant {
    
    
    public VariantOut(int typeId) {
        vt = typeId | VT_BYREF;
    }
}
