package com.example;

import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import com.example.hexdump.HexDump;

import sun.misc.Unsafe;

public class UnsafeExample {

    public Unsafe getUnsafe() {
        Unsafe result = null;

        // constructor is private
        // Unsafe u = new Unsafe();    

        // The (public) getter is secured 
        // Exception in thread "main" java.lang.SecurityException: Unsafe
        // Unsafe u = Unsafe.getUnsafe();    

        // java.lang.IllegalAccessException: Class com.example.UnsafeExample can not access a member of class sun.misc.Unsafe with modifiers "private"
/*        Class<Unsafe> c = Unsafe.class;
        try {
            Unsafe u = c.newInstance();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        }*/

        Class<Unsafe> c = Unsafe.class;
        try {
            Constructor<Unsafe> cons = c.getDeclaredConstructor();
            
            // java.lang.IllegalAccessException: Class com.example.UnsafeExample can not access a member of class sun.misc.Unsafe with modifiers "private"
            // => Need to set "Accessible" to true
            cons.setAccessible(true);

            result = cons.newInstance();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (SecurityException e) {
            e.printStackTrace();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }

/*
        Class<Unsafe> c = Unsafe.class;
        try {
            Field f = c.getDeclaredField("theUnsafe");

            // java.lang.IllegalAccessException: Class com.example.UnsafeExample can not access a member of class sun.misc.Unsafe with modifiers "private static final"
            // => Need to set "Accessible" to true
            f.setAccessible(true);

            result = f.get(c);
            System.err.println(o);
        } catch (SecurityException e) {
            e.printStackTrace();
        } catch (NoSuchFieldException e) {
            e.printStackTrace();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        }
*/

        return result;
    }

    
    public void run() {
        Unsafe unsafe = getUnsafe();
        System.err.println("Unsafe: " + unsafe);

        unsafe.putByte(0x0L, (byte) 0x0);

        byte[] data = new byte[256]; 
        Long o = new Long(0x123456789abcdef0L);
        Long o2 = new Long(0xf0edcba987654321L);
        for (int i = 0;  i < 256;  i++) {
            data[i] = unsafe.getByte(o, (long) i); 
        }
        HexDump hd = new HexDump(data);
        hd.dumpAll(System.err);

        Long l = new Long(0);
        System.err.println(l);
        unsafe.putByte(l, 0x11L, (byte) 1);
        System.err.println(l);

        long clzAddr = unsafe.getLong(o, 0x08L);
        System.err.printf("%08x\n", clzAddr);

        // unsafe.getByte(arg0)
        for (int i = 0;  i < 256;  i++) {
            data[i] = unsafe.getByte(clzAddr + i); 
        }
        hd = new HexDump(data);
        hd.dumpAll(System.err);
    }
    
    public static void main(String[] args) {
        new UnsafeExample().run();
    }

}
