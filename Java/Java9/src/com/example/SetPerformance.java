package com.example;

//import javax.jmh.annotations.*;

import java.util.Set;

public class SetPerformance {

	public void run() {
		

		Set<Integer> mySet = Set.of(1230, 1231, 1232, 1233, 1234, 
								    1235, 1236, 1237, 1238, 1239);

		System.err.println("Result: " + mySet);

		Set<Integer> mySet2 = Set.of(1230, 1231, 1232, 1233, 1234, 
			    					 1235, 1236, 1237, 1238, 1239, 1240);

		System.err.println("Result2: " + mySet2);
	}

	final static Integer[] array = new Integer[] {1230, 1231};

	
//	@Benchmark
//	@Fork(1)
	public void setOfTwo(){
		Set<Integer> mySet01 = Set.of(1230, 1231);
	}

//	@Benchmark
//	@Fork(1)
	public void setOfVarargs(){
		Set<Integer> mySet02 = Set.of(array);
	}
}
