package afester.tools.base64;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.StringReader;

import com.example.hexdump.HexDump;

public class Base64Test {

    public static void main(String[] args) {
        ByteArrayOutputStream dest = new ByteArrayOutputStream();
        Base64 b64 = new Base64();
        try {
            b64.decode(new BufferedReader(new FileReader(args[0])), dest);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        HexDump hd = new HexDump(dest.toByteArray());
        hd.dumpAll(System.err);
    }
    
    public void testEncodeDecode() {
        Base64 b64 = new Base64();
        
        byte[] input = {0x5, 0x42, (byte) 0x84, (byte) 0xaf, (byte) 0xbb};
        byte[] input2 = {0x5, 0x42, (byte) 0x84, (byte) 0xaf, (byte) 0xbb, (byte) 0xff, 0x55, (byte) 0xaa};
        byte[] input3 = {0x5, 0x42, (byte) 0x84, (byte) 0xaf, (byte) 0xbb, (byte) 0xff, 0x55, (byte) 0xaa, 0x00, 0x01};
        byte[] input4 = {0x5, 0x42, (byte) 0x84, (byte) 0xaf, (byte) 0xbb, (byte) 0xff, 0x55, (byte) 0xaa, 0x00, 0x01, 0x02, 0x77, 0x66, 0x22, (byte) 0xff};

        try {
            String result = b64.encode(new ByteArrayInputStream(input));
            System.err.println(result);
            byte[] data = b64.decode(new StringReader(result));
            HexDump hd = new HexDump(data);
            hd.dumpAll(System.err);

            System.err.println("------------------");
            result = b64.encode(new ByteArrayInputStream(input2));
            System.err.println(result);
            data = b64.decode(new StringReader(result));
            hd = new HexDump(data);
            hd.dumpAll(System.err);

            System.err.println("------------------");
            result = b64.encode(new ByteArrayInputStream(input3));
            System.err.println(result);
            data = b64.decode(new StringReader(result));
            hd = new HexDump(data);
            hd.dumpAll(System.err);

            System.err.println("------------------");
            result = b64.encode(new ByteArrayInputStream(input4));
            System.err.println(result);
            data = b64.decode(new StringReader(result));
            hd = new HexDump(data);
            hd.dumpAll(System.err);
        } catch (IOException e) {
            e.printStackTrace();
        }
        
    }
}
