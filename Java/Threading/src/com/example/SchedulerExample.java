package com.example;

import java.time.LocalTime;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

public class SchedulerExample implements Runnable {

	public static void main(String[] args) {
		SchedulerExample example = new SchedulerExample();
		example.start();
	}
	
	private void start() {
		ScheduledExecutorService executor = Executors.newSingleThreadScheduledExecutor();
		executor.scheduleAtFixedRate(this, 0, 1, TimeUnit.SECONDS);
		//executor.scheduleWithFixedDelay(this, 0, 1, TimeUnit.SECONDS);
	}

	@Override
	public void run() {
		System.err.println(LocalTime.now() + " Hello ...");
		try {
			Thread.sleep(200);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

}
