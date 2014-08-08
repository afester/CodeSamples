package com.example.hexdump;

import java.io.PrintStream;

/**
 * Provides a hex dump facility which can be used to dump memory blocks to
 * 
 * @author afester
 *
 */
public class HexDump {

    private byte[] data;
    private int offset = 0;
    private int bytesPerLine = 16;
    private String prefix = "";

    public HexDump(byte[] data) {
        this.data = data;
    }

    
    public void setPrefix(String prefix) {
        this.prefix = prefix;
    }


    public boolean hasNext() {
        if (offset < data.length) {
            return true;
        }
        return false;
    }

    public String nextLine() {
        StringBuffer result = new StringBuffer(80);

        // prefix
        result.append(prefix);

        // address
        result.append(String.format("%04X:", offset));

        // hex dump
        int hexIdx = 0;
        for (hexIdx = offset; hexIdx < offset + bytesPerLine
                && hexIdx < data.length; hexIdx++) {
            result.append(String.format(" %02X", (int) data[hexIdx] & 0xFF));
        }

        // spacer
        for (; hexIdx < offset + bytesPerLine; hexIdx++) {
            result.append("   ");
        }

        // ascii dump
        result.append("  ");
        for (int idx = offset; idx < offset + bytesPerLine && idx < data.length; idx++) {
            int c = (int) data[idx] & 0xFF;
            if (c < 32) { // c == '\n' || c == '\t') {
                c = '.';
            }
            result.append(String.format("%c", c));
        }

        offset += bytesPerLine;
        return result.toString();
    }


    public void dumpAll(PrintStream dest) {
        while (hasNext()) {
            dest.println(nextLine());
        }
    }


    // test method
    public static void main(String[] args) {
        HexDump hd1 = new HexDump(new byte[] { 1, 2, 3 });
        while (hd1.hasNext()) {
            String nextLine = hd1.nextLine();
            System.err.println(nextLine);
        }

        HexDump hd2 = new HexDump(new byte[] { 1, 2, 3, (byte) 0xff,
                (byte) 254, (byte) 253, (byte) 252, (byte) 240, 64, 3, 4, 5, 6,
                34, 66, 23, 7, 5, 1, 121, 66, 77, 88, 99, 2, 4, 8, 16, 32, 64,
                (byte) 128, 25, 121, 0, 17, 43 });
        hd2.setPrefix("> ");
        while (hd2.hasNext()) {
            String nextLine = hd2.nextLine();
            System.err.println(nextLine);
        }
    }
}
