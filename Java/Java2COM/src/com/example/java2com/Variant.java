package com.example.java2com;

public class Variant {

    public Variant() {
    }

    public Variant(boolean b) {
        this.vt = VT_BOOL;
        this.booleanValue = b;
    }

    public Variant(String s) {
        this.vt = VT_BSTR;
        this.strValue = s;
    }

    public Variant(int i) {
        this.vt = VT_I4;
        this.intValue = i;
    }

    public String stringValue() {
        String result = "";
        switch(vt) {
            case VT_BOOL :
                if (booleanValue) {
                    result = "true";
                } else {
                    result = "false";
                }
                break;

            case VT_I4 :
                result = String.valueOf(intValue);
                break;

            case VT_BSTR :
                result = strValue;
                break;

            case VT_DISPATCH :
                result = dispatch.toString();
                break;
        }
        
        return result;
    }


    public String toString() {
        return "Variant[type=" + vt + ", value=" + stringValue(); 
    }

    // See http://msdn.microsoft.com/en-us/library/ms221170(v=vs.85).aspx
    public static final short VT_EMPTY              = 0;
    public static final short VT_NULL               = 1;
    public static final short VT_I2                 = 2;
    public static final short VT_I4                 = 3;
    public static final short VT_R4                 = 4;
    public static final short VT_R8                 = 5;
    public static final short VT_CY                 = 6;
    public static final short VT_DATE               = 7;
    public static final short VT_BSTR               = 8;
    public static final short VT_DISPATCH           = 9;
    public static final short VT_ERROR              = 10;
    public static final short VT_BOOL               = 11;
    public static final short VT_VARIANT            = 12;
    public static final short VT_UNKNOWN            = 13;
    public static final short VT_DECIMAL            = 14;
    public static final short VT_I1                 = 16;
    public static final short VT_UI1                = 17;
    public static final short VT_UI2                = 18;
    public static final short VT_UI4                = 19;
    public static final short VT_I8                 = 20;
    public static final short VT_UI8                = 21;
    public static final short VT_INT                = 22;
    public static final short VT_UINT               = 23;
    public static final short VT_VOID               = 24;
    public static final short VT_HRESULT            = 25;
    public static final short VT_PTR                = 26;
    public static final short VT_SAFEARRAY          = 27;
    public static final short VT_CARRAY             = 28;
    public static final short VT_USERDEFINED        = 29;
    public static final short VT_LPSTR              = 30;
    public static final short VT_LPWSTR             = 31;
    public static final short VT_RECORD             = 36;
    public static final short VT_INT_PTR            = 37;
    public static final short VT_UINT_PTR           = 38;
    public static final short VT_FILETIME           = 64;
    public static final short VT_BLOB               = 65;
    public static final short VT_STREAM             = 66;
    public static final short VT_STORAGE            = 67;
    public static final short VT_STREAMED_OBJECT    = 68;
    public static final short VT_STORED_OBJECT      = 69;
    public static final short VT_BLOB_OBJECT        = 70;
    public static final short VT_CF                 = 71;
    public static final short VT_CLSID              = 72;
    public static final short VT_VERSIONED_STREAM   = 73;
    public static final short VT_BSTR_BLOB          = 0xfff;
    public static final short VT_VECTOR             = 0x1000;
    public static final short VT_ARRAY              = 0x2000;
    public static final short VT_BYREF              = 0x4000;

    // data type
    public int vt;

    // Various data type values
    public String strValue;
    public IDispatch dispatch;
    public int intValue;
    public boolean booleanValue;
}
