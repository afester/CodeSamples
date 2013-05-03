package com.example;
import javax.annotation.Generated;


public class AnnotationsSample {

	// simple annotation
	@Override
	public boolean equals(Object obj) {
		return super.equals(obj);
	}

	// Annotation with an element
	@Generated(value="com.example.Generator")
	public void generatedMethod() {
	}
	
	// Custom annotation
	@Author(name="Andreas Fester", 
			  eMail="Andreas.Fester@gmx.de")
	public void additionalMethod() {
	}
}
