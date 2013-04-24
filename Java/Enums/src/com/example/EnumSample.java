package com.example;

import java.util.Arrays;

public class EnumSample {

	public static void main(String[] args) {
		System.out.println("enum as array:");
		System.out.println("   " + Arrays.toString(Color.values()) + "\n");

		System.out.println("All enum constants:");
		for (Color c : Color.values()) {
			System.out.println("   " + c.name() + "=" + c.ordinal());	
		}

		System.out.println("\nenum from a String:");
		String color = "YELLOW";
		Color c = Color.valueOf(color);
		System.out.println("   " + c.name() + "=" + c.ordinal() + " RGB=0x" + Integer.toHexString(c.getRGB()));
		
		System.out.println("\nenum from unknown String:");
		String color2 = "yellow";
		try {
			Color c2 = Color.valueOf(color2); // throws "IllegalArgumentException"
			System.out.println("   " + c2.name() + "=" + c2.ordinal() + " RGB=0x" + Integer.toHexString(c2.getRGB()));
		}catch(IllegalArgumentException iae) {
			System.out.println("   \"" + color2 + "\" not found in enum.");
		}
		
	}
}
