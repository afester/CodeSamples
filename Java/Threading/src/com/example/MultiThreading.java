package com.example;

public class MultiThreading {
	public static void main(String[] args) {

		// the most simple threading example. 
		// non-synchronized and not much useful ...
		Thread t1 = new Thread(() -> { for (int i = 0;  i < 10;  i++) System.err.println("Thread1"); } );
		Thread t2 = new Thread(() -> { for (int i = 0;  i < 10;  i++) System.err.println("Thread2"); } );

		t2.start();
		t1.start();
	}
}
