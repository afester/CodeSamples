package com.example.log4j;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class SampleLog4j {

    private Logger l = LogManager.getLogger(SampleLog4j.class);

    public void run() {
        l.info("Application Running.");
    }

    public static void main(String[] args) {
        // Define a configuration file to use for the Log4j API
        // See http://logging.apache.org/log4j/2.x/manual/configuration.html
        System.setProperty("log4j.configurationFile", "log4j.xml");

        new SampleLog4j().run();
    }

}
