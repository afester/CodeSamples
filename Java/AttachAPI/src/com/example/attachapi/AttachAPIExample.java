package com.example.attachapi;

import java.io.File;
import java.io.IOException;
import java.nio.file.FileSystems;
import java.nio.file.Path;
import java.util.Optional;
import java.util.Properties;

import com.sun.tools.attach.AgentInitializationException;
import com.sun.tools.attach.AgentLoadException;
import com.sun.tools.attach.AttachNotSupportedException;
import com.sun.tools.attach.VirtualMachine;
import com.sun.tools.attach.VirtualMachineDescriptor;

public class AttachAPIExample {

    public static void main(String[] args) {
        try {
            System.err.println("List of available VMs:");
            VirtualMachine.list().forEach(e -> {
               System.err.printf("%6s: %s\n", e.id(), e.displayName());
            });
            Optional<VirtualMachineDescriptor> vmd = 
                    VirtualMachine.list()
                                  .stream()
                                  .filter(e -> e.displayName().contains("BreadBoardEditor"))
                                  .findFirst();
            if (vmd.isEmpty()) {
                System.err.printf("No VM found!");
                System.exit(0);
            }
            final String vmId = vmd.get().id();

            // get path to testAgent.jar
            final Path path = FileSystems.getDefault().getPath("").toAbsolutePath();
            final String agentJarFile = path.toString() + File.separator + "testAgent.jar";
            System.err.printf("Loading %s into target VM ...\n", agentJarFile);

            VirtualMachine vm = VirtualMachine.attach(vmId);
            
            Properties props = vm.getSystemProperties();
            props.list(System.err);
            
            
            
            vm.loadAgent(agentJarFile, null);
            System.err.printf("Loaded.\n");
            
            System.err.printf("Detaching ...\n");
            vm.detach();
        } catch (AttachNotSupportedException | IOException | AgentLoadException | AgentInitializationException e) {
            e.printStackTrace();
        }
    }
}
