package com.example;

import java.nio.charset.Charset;

public class DumpFileEncoding {
    public static void main(String[] args) {
        System.err.printf("file.encoding  : %s\n", System.getProperty("file.encoding"));
        System.err.printf("Default Charset: %s\n", Charset.defaultCharset());
    }
}
