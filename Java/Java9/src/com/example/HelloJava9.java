package com.example;

public class HelloJava9 {

	public static void main(String[] args) {
		String version = System.getProperty("java.runtime.version");
		System.err.printf("Java version: %s%n", version);
		new SetPerformance().run();
	}
}
