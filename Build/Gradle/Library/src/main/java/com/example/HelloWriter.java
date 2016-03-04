package com.example;

import org.apache.logging.log4j.Logger;
import org.apache.logging.log4j.LogManager;


public class HelloWriter {
  private static final Logger logger = LogManager.getLogger();

  public void sayHello(String to) {
    logger.info("Saying a greeting ...");

    System.err.printf("Hello %s\n", to);
  }
}
