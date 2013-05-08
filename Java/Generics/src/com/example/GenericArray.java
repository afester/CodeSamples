package com.example;

import java.util.LinkedList;

public class GenericArray {

	public static void main(String[] args) {

		// Cannot create a generic array of LinkedList<String>
		// See also http://stackoverflow.com/questions/2927391/whats-the-reason-i-cant-create-generic-array-types-in-java

//		LinkedList<String>[] buckets = new LinkedList<String>[5];
		   // ERROR: "Cannot create a generic array of LinkedList<String>"

	}
}
