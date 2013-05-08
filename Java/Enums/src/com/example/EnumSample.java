package com.example;

import java.util.Arrays;

public class EnumSample {

   private Integer getIndex() {
      return 7;
   }
   
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


		// Sample of mapping an int to an enum:

		EnumSample service = new EnumSample();

		switch(Index.valueOf(service.getIndex())) {
		   case ONE : System.out.println("ONE"); break;
		   case TWO : System.out.println("TWO"); break;
		   case INVALID : System.out.println("INVALID"); break;
		   
         case REG:
            break;
            
         case THREE:
            break;
            
         case ZERO:
            break;
            
         default:
            break;
		}
	}
}
