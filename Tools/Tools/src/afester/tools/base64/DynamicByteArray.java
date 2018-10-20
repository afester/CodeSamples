package afester.tools.base64;

import java.util.Arrays;

public class DynamicByteArray {

    private byte[] data;

    public DynamicByteArray() {
        data = new byte[1024];
    }

    public DynamicByteArray(int initialSize) {
        data = new byte[initialSize];
    }
    
    public byte[] getArray() {
        return data;
    }
    
    public byte get(int idx) {
        return data[idx];
    }

    public void set(int idx, byte value) {
        if (idx >= data.length) {
            resize(data.length + 1024);
        }

        data[idx] = value;
    }
    
    public void resize(int size) {
        data = Arrays.copyOf(data, size);
    }
}
