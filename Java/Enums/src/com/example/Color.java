package com.example;

// http://docs.oracle.com/javase/tutorial/java/javaOO/enum.html
// http://docs.oracle.com/javase/7/docs/api/java/lang/Enum.html
public enum Color { // implicitly extends java.lang.Enum
	RED(0xff0000),
	GREEN(0x00ff00),
	BLUE(0x0000ff),
	YELLOW(0xffff00),
	ORANGE(0xffa500);

	// additional fields and methods must follow *after* the enum constants
	private int rgbValue;

	/**
	 * The constructor for an enum type must be package-private or private 
	 * access. It automatically creates the constants that are defined at 
	 * the beginning of the enum body. 
	 * You cannot invoke an enum constructor yourself.
	 */
	private Color(int anRgbValue) {
		rgbValue = anRgbValue;
	}
	

	public int getRGB() {
		return rgbValue;
	}
}
