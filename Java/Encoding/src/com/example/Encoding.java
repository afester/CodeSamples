package com.example;

import java.io.UnsupportedEncodingException; 
import java.nio.charset.Charset;


public class Encoding {

	
	
	/**
	 * @param args
	 * @throws UnsupportedEncodingException 
	 */
	public static void main(String[] args) {
		try{
			Encoding e = new Encoding();
			e.run();
		} catch(Exception e) {
			e.printStackTrace();
		}
	}

	
	public void run() throws UnsupportedEncodingException {
		dumpDefaults();
		testSimple();
		testSimpleUmlaut();
	}
	
	
	
	private void testSimpleUmlaut() throws UnsupportedEncodingException {
		System.out.println("Simple umlauts test:");

                        // D      ü      r    r    e
		byte[] rawdata = {68, -61, -68, 114, 114, 101};
                           // 0xC3, 0xBC = "ü" in UTF-8
        System.out.println("   Input    : " + arrayToHex(rawdata) + "\n");

        String s = new String(rawdata, "ISO-8859-1");
        System.out.println("   String   : " + s);

        byte[] iso8859_1 = s.getBytes("ISO-8859-1");
        System.out.println("   ISO8859-1: " + arrayToHex(iso8859_1));
        byte[] utf16 = s.getBytes("UTF-16");
        System.out.println("   UTF-16   : " + arrayToHex(utf16));
        byte[] utf8 = s.getBytes("UTF-8");
        System.out.println("   UTF-8    : " + arrayToHex(utf8));
        byte[] usAscii = s.getBytes("US-ASCII");
        System.out.println("   US-ASCII : " + arrayToHex(usAscii) + "\n");

        System.out.println(" Converting ...");
        String s2 = new String(iso8859_1, "UTF-8");
        System.out.println("   String    : " + s2);
        iso8859_1 = s2.getBytes("ISO-8859-1");
        System.out.println("   ISO8859-1 : " + arrayToHex(iso8859_1));
        utf16 = s2.getBytes("UTF-16");
        System.out.println("   UTF-16    : " + arrayToHex(utf16));
        utf8 = s2.getBytes("UTF-8");
        System.out.println("   UTF-8     : " + arrayToHex(utf8));
        usAscii = s2.getBytes("US-ASCII");
        System.out.println("   US-ASCII  : " + arrayToHex(usAscii));
        byte[] iso8859_15 = s2.getBytes("ISO-8859-15");
        System.out.println("   ISO8859-15: " + arrayToHex(iso8859_15));
        
		System.out.println("---------------------------------------------------------");
	}


	private void testSimple() throws UnsupportedEncodingException {
		System.out.println("Simple test:");

		String s = "Hello World";
		System.out.println("   String: \"" + s + "\"");

		byte[] utf16 = s.getBytes("UTF-16");
		System.out.println("   UTF16 : " + arrayToHex(utf16));
		
		byte[] utf8 = s.getBytes("UTF-8");
		System.out.println("   UTF8  : " + arrayToHex(utf8));

		byte[] usAscii = s.getBytes("US-ASCII");
		System.out.println("   ASCII : " + arrayToHex(usAscii));

		System.out.println("---------------------------------------------------------");
	}


	private void dumpDefaults() {
		/*
		user.country.format=DE
		user.country=US
		user.language=en
		user.language.format=de
		*/
		System.out.println("Encoding defaults:");
		System.out.println("   file.encoding.pkg=" + System.getProperty("file.encoding.pkg"));
		System.out.println("   sun.jnu.encoding=" + System.getProperty("sun.jnu.encoding"));
		System.out.println("   file.encoding=" + System.getProperty("file.encoding"));
		System.out.println("   sun.io.unicode.encoding=" + System.getProperty("sun.io.unicode.encoding"));
		System.out.println("   Default Charset: " + Charset.defaultCharset().name());
		System.out.println("---------------------------------------------------------");
	}

	
	private String arrayToHex(byte[] array) {
		return "[" + getHexString(array) + "]";
	}
	
	private String getHexString(byte[] array) {
		String result = "";
		for (byte b : array) {
			int val = ((int) b) & 0xff;
			String s = Integer.toHexString(val);
			if (s.length() < 2) {
				s = "0" + s;
			}
			if (result.length() > 0) {
				result = result + " ";
			}
			result = result + s;
			
		}
		return result;
	}
}
