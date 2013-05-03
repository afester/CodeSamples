package com.example;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;
import java.lang.annotation.ElementType;

// Need to use RetentionPolicy.RUNTIME to be able to read the annotation
// via reflection
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.METHOD)
public @interface Author {

	String name() default "Anonymous";
	String eMail();
}
