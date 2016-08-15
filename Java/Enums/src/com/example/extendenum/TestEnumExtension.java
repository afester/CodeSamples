package com.example.extendenum;

public class TestEnumExtension {
    
    
    
    public static void main(String[] args) {
        new TestEnumExtension().run();
    }

    private void printEnum(ExtensibleEnum val) {
        System.err.printf("%s: %s\n", val.name(), val.ordinal());
    }

    private void run() {
        printEnum(DefaultValues.VALUE_ONE);
        printEnum(DefaultValues.VALUE_TWO);
        printEnum(ExtendedValues.VALUE_THREE);
        printEnum(ExtendedValues.VALUE_FOUR);
        printEnum(ExtendedValues.VALUE_ONE);    // !!
    }
}
