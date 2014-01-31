package com.example.agent;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintStream;
import java.lang.instrument.Instrumentation;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.URL;
import java.security.CodeSource;
import java.security.ProtectionDomain;

public class Agent implements Runnable {

    private static final int LISTEN_PORT = 4711;

    private Instrumentation theInstrumentation;
    private ServerSocket serverSocket;
    private Socket dataSocket;
    private PrintStream ps;

    // TODO: How to stop the agent once the application is about to terminate? 
    public static void premain(String args, Instrumentation inst) {
        Agent a = new Agent();
        a.start(inst);
    }


    private void start(Instrumentation inst) {
        System.out.println("Agent.start()");
        theInstrumentation = inst;

        try {
            serverSocket = new ServerSocket(LISTEN_PORT);
        } catch (IOException e) {
            e.printStackTrace();
        }

        new Thread(this).start();
    }


    @Override
    public void run() {
        while(true) {
            System.out.println("Waiting for connection ...");
            try {
                dataSocket = serverSocket.accept();
                System.out.println("Connected.");

                // set up the input stream
                InputStream is = dataSocket.getInputStream();
                InputStreamReader isr = new InputStreamReader(is);
                BufferedReader br = new BufferedReader(isr);

                // set up the output stream
                OutputStream os = dataSocket.getOutputStream();
                ps = new PrintStream(os);
                String command = "";
                while(!"quit".equals(command)) {
                    ps.print("Agent> ");
                    command = br.readLine().trim();
                    switch(command) {
                        case "classes" : 
                            printClasses();
                            break;
                    }
                    System.out.println(command);
                }
                dataSocket.close();
                dataSocket = null;
                ps = null;
            } catch (IOException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }
    }


    private void printClasses() {
        Class<?>[] classes = theInstrumentation.getAllLoadedClasses();
        for (Class<?> c : classes) {
            String className = c.getName();
            String codeSource = "<unknown>";
            ProtectionDomain pd = c.getProtectionDomain();
            if (pd != null) {
                codeSource = "<unknown> (pd!=null)";
                CodeSource cs = pd.getCodeSource();
                if (cs != null) {
                    codeSource = "<unknown> (cs!=null)";
                    URL location = cs.getLocation();
                    if (location != null) {
                        codeSource = "<unknown> (location!=null)";
                        codeSource = location.toString();
                    }
                } else {
                    String resource = c.getName();
                    resource = resource.replace('.', '/');
                    resource = resource + ".class";
                    URL location = ClassLoader.getSystemClassLoader().getResource(resource);
                    // codeSource = resource;
                    if (location != null) {
                        codeSource = location.toString();
                    }
                }
            }

            ps.println(className + ": " + codeSource);
        }
    }
}
