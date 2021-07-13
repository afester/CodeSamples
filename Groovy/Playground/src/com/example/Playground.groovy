package com.example

class Playground {

	public static void main(args) {
		args.each { println(it) }

		byte value = 243
		println value
		
		def val1 = "Hello"
		def val2 = "World"
		def val3 = val1 + val2
		println val3
		println "----------------"
		
		def values = 1..13
		println values	
		println "----------------"
		
		values.each { println(it) }
		println "----------------"

		(5..10).each { println(it) }
		println "----------------"
		
		staticMethod("Hello World")
		staticMethod(3.1415)
		println "----------------"

		Playground thePlayground = new Playground()
		thePlayground.helloWorld();
		println "----------------"

		// Note to pass the JavaFX main class as first parameter to launch()!
		JavaFXSample.launch(JavaFXSample, args)
	}

	public static def staticMethod(param) {
		println param
	}

	public void helloWorld() {
		println "Hello World"
	}
}
