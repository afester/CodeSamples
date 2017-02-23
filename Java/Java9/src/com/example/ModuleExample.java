package com.example;

import com.example.lib.SampleLibrary;

public class ModuleExample {
	public static void main(String[] args) {
		new ModuleExample().run();
	}
	
	
	public void run() {
		SampleLibrary sl = new SampleLibrary();
		sl.sayHelloTo("Test");
	}
}
