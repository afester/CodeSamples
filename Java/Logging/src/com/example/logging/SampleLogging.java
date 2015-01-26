package com.example.logging;

import java.util.logging.Logger;

public class SampleLogging {

    private Logger l = Logger.getLogger("com.example.logging.SampleLogging");

    public void run() {
        l.info("Application Running.");
    }

    public static void main(String[] args) {
        // Define a configuration file to use for the java.util.logging API
        // See https://docs.oracle.com/javase/8/docs/api/java/util/logging/LogManager.html
        System.setProperty("java.util.logging.config.file", "utillogging.properties");

        new SampleLogging().run();
    }

}
