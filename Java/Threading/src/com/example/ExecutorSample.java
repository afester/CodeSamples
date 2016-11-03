package com.example;

import java.util.concurrent.ExecutionException;
import java.util.concurrent.Executor;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

// An Executor is normally used instead of explicitly creating threads.

public class ExecutorSample {

	public static void main(String[] args) {
		// get an executor
		//Executor executor = Executors.newCachedThreadPool(); 		// unbounded thread pool, with automatic thread reclamation
		Executor executor = Executors.newFixedThreadPool(3); 		// fixed size thread pool
		//Executor executor = Executors.newSingleThreadExecutor();	// single background thread

		// Execute a task at a later time
		executor.execute(() -> { for (int i = 0;  i < 10;  i++) System.err.println("Thread1"); } );
		executor.execute(() -> { for (int i = 0;  i < 10;  i++) System.err.println("Thread2"); } );

		// Submit a task for later execution and retrieve a Future to get the result 
		ExecutorService execService = Executors.newFixedThreadPool(3);
		
		// return the result given as second parameter
		Future<Integer> futureResult = execService.submit(() -> { for (int i = 0;  i < 10;  i++) System.err.println("Thread2.1"); }, Integer.valueOf(42));
		
		// Use a Callable which can return its own result
		Future<Integer> futureResult2 = execService.submit(() -> { for (int i = 0;  i < 10;  i++) System.err.println("Thread2.22"); return 55; } );

		// Just use a Runnable, with no result
		// There is still a Future returned, which can be used to wait for task
		// completion, but its get()-Method returns null
		Future<?> futureResult3 = execService.submit(() -> { for (int i = 0;  i < 10;  i++) System.err.println("Thread2.33"); } );

		System.err.println("DONE scheduling");

		try {
			Integer result = futureResult.get();
			System.err.println("  result = " + result);
			Integer result2 = futureResult2.get();
			System.err.println("  result2 = " + result2);
			Object result3 = futureResult3.get();
			System.err.println("  result3 = " + result3);
		} catch (InterruptedException | ExecutionException e) {
			e.printStackTrace();
		}

		System.err.println("DONE main");
	}
}
