package com.example.testagent;

import java.lang.instrument.Instrumentation;

public class TestAgent {

    public static void agentmain(String agentArgs, Instrumentation inst) {
        // Note: this is written into the target VM's stderr!!!
        System.err.println("ATTACHED!");
        System.err.printf("MAX: %s, FREE: %s, TOTAL: %s\n", Runtime.getRuntime().maxMemory(),
                            Runtime.getRuntime().freeMemory(),
                            Runtime.getRuntime().totalMemory());
    }
}
