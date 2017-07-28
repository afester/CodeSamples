package com.example;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import com.example.lib.SampleLibrary;


public class ModuleExample {
	public static void main(String[] args) {
		new ModuleExample().run();
	}
	
	
	public void run() {
		SampleLibrary sl = new SampleLibrary();
		sl.sayHelloTo("Test");

		// sl.protectedSayHello();		// Method not visible

		try {
			Method prot = SampleLibrary.class.getDeclaredMethod("protectedSayHello");
			prot.setAccessible(true);
			prot.invoke(sl);
		} catch (NoSuchMethodException | SecurityException | IllegalAccessException | IllegalArgumentException | InvocationTargetException e) {
			e.printStackTrace();
		}
	}
}
