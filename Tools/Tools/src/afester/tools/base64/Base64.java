package afester.tools.base64;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.Reader;
import java.io.StringReader;

public class Base64 {

    public Base64() {
    }
    
    
    /**
     * Decodes a BASE64 input stream into a byte array. 
     *
     * @param input The BASE64 encoded input stream.
     * @return A byte array which contains the binary representation of the input.
     *
     * @throws IOException If an error occurred when reading from the input stream.
     */
    public byte[] decode(Reader input) throws IOException {
        char[] encodedChunk = new char[4];

        DynamicByteArray result = new DynamicByteArray();
        int idx = 0;
        while(true) {
            int numChars = input.read(encodedChunk);

            if (numChars <= 0) {
                result.resize(idx);
                return result.getArray();
            }

            // number of input characters MUST be multiple of 4 
            if (numChars != 4) {
                throw new RuntimeException("Invalid input length: " + numChars);
            }

            if (encodedChunk[2] == '=' && encodedChunk[3] == '=' ) {
                //    00aaaaaa 00aa0000 ======== ========
                // => aaaaaaaa

                result.set(idx++, (byte) (getInt(encodedChunk[0]) << 2 | getInt(encodedChunk[1]) >> 4));

                result.resize(idx);
                return result.getArray();
            } else if (encodedChunk[3] == '=' ) {
                //    00aaaaaa 00aabbbb 00bbbb00 ========
                // => aaaaaaaa bbbbbbbb

                result.set(idx++, (byte) (getInt(encodedChunk[0]) << 2 | getInt(encodedChunk[1]) >> 4)); 
                result.set(idx++, (byte) (getInt(encodedChunk[1]) << 4 | getInt(encodedChunk[2]) >> 2)); 

                result.resize(idx);
                return result.getArray();
            } else {
                //    00aaaaaa 00aabbbb 00bbbbcc 00cccccc
                // => aaaaaaaa bbbbbbbb cccccccc
                
                result.set(idx++, (byte) (getInt(encodedChunk[0]) << 2 | getInt(encodedChunk[1]) >> 4));
                result.set(idx++, (byte) (getInt(encodedChunk[1]) << 4 | getInt(encodedChunk[2]) >> 2)); 
                result.set(idx++, (byte) (getInt(encodedChunk[2]) << 6 | getInt(encodedChunk[3]))); 
            }
        }
    }


    /**
     * Encodes a binary input stream into an BASE64 encoded String. 
     *
     * @param input The binary input stream.
     * @return The String which represents the data from the input stream in BASE64.
     *
     * @throws IOException If an error occurred when reading from the input stream.
     */
    public String encode(InputStream input) throws IOException {
        byte[] chunk = new byte[3];
        char[] encodedChunk = new char[4];
        StringBuffer result = new StringBuffer(); 

        while(true) {
            // read up to three bytes
            int numBytes = input.read(chunk, 0,  3);
            if (numBytes <= 0) {
                return result.toString();
            }

            // encode read bytes into 4 characters
            encodedChunk[0] = getChar(  (chunk[0] & 0xff) >> 2);
            if (numBytes == 1) {
                //    aaaaaaaa
                // => 00aaaaaa 00aa0000 ======== ========
                encodedChunk[1] = getChar(  (chunk[0] & 0x03) << 4);
                encodedChunk[2] = '=';
                encodedChunk[3] = '=';

                result.append(encodedChunk,  0, encodedChunk.length);
                return result.toString();
            } else if (numBytes == 2) {
                //    aaaaaaaa bbbbbbbb
                // => 00aaaaaa 00aabbbb 00bbbb00 ========
                encodedChunk[1] = getChar( ((chunk[0] & 0x03) << 4) | ((chunk[1] & 0xff) >> 4));
                encodedChunk[2] = getChar(  (chunk[1] & 0x0f) << 2);
                encodedChunk[3] = '=';
                
                result.append(encodedChunk,  0, encodedChunk.length);
                return result.toString();
            } else if (numBytes == 3) {
                //    aaaaaaaa bbbbbbbb cccccccc
                // => 00aaaaaa 00aabbbb 00bbbbcc 00cccccc
                encodedChunk[1] = getChar( ((chunk[0] & 0x03) << 4) | ((chunk[1] & 0xff) >> 4));
                encodedChunk[2] = getChar( ((chunk[1] & 0x0f) << 2) | ((chunk[2] & 0xff) >> 6));
                encodedChunk[3] = getChar(   chunk[2] & 0x3f);

                result.append(encodedChunk,  0, encodedChunk.length);
            }
        }
    }


    /**
     * 
     * @param source The buffered reader from where to read the lines
     * @param dest The destination stream for the decoded data
     */
    public void decode(BufferedReader source, OutputStream dest) throws IOException {
        source.lines().forEach(l -> {
            l = l.trim();
            if (!l.startsWith("-----")) {
                try {
                    dest.write(decode(new StringReader(l)));
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
    }


    /**
     * Maps a number between 0 .. 63 to an ascii character.
     *
     * @param number The number to map.
     * @return The ASCII character for that number.
     */
    private char getChar(int number) {
        if (number >= 0 && number < 26) {
            return (char) ('A' + number);
        }
        if (number >= 26 && number < 52) {
            return (char) ('a' + (number - 26));
        }
        if (number >= 52 && number < 62) {
            return (char) ('0' + (number - 52));
        }
        if (number == 62) {
            return '+';
        }
        if (number == 63) {
            return '/';
        }

        throw new RuntimeException(String.format("Encoding error: %s >= 63", number));
    }

    /**
     * Maps an ASCII character [A..Za..z0..9+/] into a number between 0 .. 63
     *
     * @param c  The character to map.
     * @return   The mapped number in the range [0..63] 
     */
    private int getInt(char c) {
        if (c >= 'A' && c <= 'Z') {
            return c - 'A';
        }
        if (c >= 'a' && c <= 'z') {
            return c - 'a' + 26;
        }
        if (c >= '0' && c <= '9') {
            return c - '0' + 52;
        }
        if (c == '+') {
            return 62;
        }
        if (c == '/') {
            return 63;
        }

        throw new RuntimeException(String.format("Decoding error: Invalid character %s", c));
    }
}
