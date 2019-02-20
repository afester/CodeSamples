package com.example;

import org.apache.poi.hdgf.dev.VSDDumper;

public class Main {
    public static void main(String[] args) {
/*
        try {
            VSDDumper.main(new String[] {"Simple.vsd"});
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
*/
        // 40 15 8B C5 62 B1 58 - 45
        // 40 3E 9F CF E7 F3 F9 - 46
        
        // CHUNK:XForm/155
        // 0000: 40 00 00 00 00 00 00 00 00 40 DE EF F7 FB FD 7E  @........@Юпчыэ~
        // 0010: 1F 40 46 B9 5C 2E 97 CB E5 02 40 46 BA 5C 2E 97  .@F№\.?Ле.@Fє\.?
        // 0020: CB E5 F2 3F 46 B9 5C 2E 97 CB E5 F2 3F 46 BA 5C  Лет?F№\.?Лет?Fє\
        // 0030: 2E 97 CB E5 E2 3F 51 00 00 00 00 00 00 00 00 00  .?Лев?Q.........
        VisioFile vf = new VisioFile("Simple.vsd");
        vf.extractComponents();
    }
}
