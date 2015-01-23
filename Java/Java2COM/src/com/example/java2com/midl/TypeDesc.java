package com.example.java2com.midl;

public class TypeDesc {

//    private boolean voidType = true;
//    private boolean simpleType = false;
    private String name = "void";

    public TypeDesc() {
    }

    public TypeDesc(String name) {
//        this.voidType = false;
        this.name = name;
    }
/*
    public boolean isVoid() {
        return voidType;
    }
  */  
    public boolean isSimple() {
        if ("char".equals(name) ||
            "long".equals(name) ||
            "short".equals(name) ||
            "double".equals(name) ||
            "VARIANT_BOOL".equals(name)) {
            return true;
        }

        return false;
    }
    
    public String getName() {
        if ("BSTR".equals(name)) {
            return "String";
        }
        if ("short".equals(name)) {
            return "int";
        }
        if ("VARIANT_BOOL".equals(name)) {
            return "boolean";
        }
        if ("VARIANT".equals(name)) {
            return "Variant";
        }
        if ("OLE_COLOR".equals(name)) {
            return "java.awt.Color";
        }
        if ("DATE".equals(name)) {
            return "java.util.Date";
        }
        if ("IFontDisp".equals(name) ||
            "IPictureDisp".equals(name)) {
            return "Object";
        }

        return name;
    }
}
