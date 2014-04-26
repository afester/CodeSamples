package com.example.agent;

import java.lang.instrument.ClassFileTransformer;
import java.lang.instrument.IllegalClassFormatException;
import java.lang.instrument.Instrumentation;
import java.security.ProtectionDomain;

import org.objectweb.asm.ClassReader;
import org.objectweb.asm.ClassVisitor;
import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;

import static org.objectweb.asm.Opcodes.*;

class RemoveNopAdapter extends MethodVisitor {
   public RemoveNopAdapter(MethodVisitor mv) {
      super(ASM4, mv);
   }

   @Override
   public void visitInsn(int opcode) {
      System.err.println("OP: " + opcode);
      mv.visitInsn(opcode);
//      if (opcode != NOP) {
//         mv.visitInsn(opcode);
//      }
   }
}

class ChangeVersionAdapter extends ClassVisitor {
   public ChangeVersionAdapter(ClassVisitor cv) {
      super(ASM4, cv);
   }

   @Override
   public MethodVisitor visitMethod(int access, String name,
               String desc, String signature, String[] exceptions) {
      MethodVisitor mv;
      mv = cv.visitMethod(access, name, desc, signature, exceptions);
      if (mv != null) {
         System.err.println(name);
         mv = new RemoveNopAdapter(mv);
      }
      return mv;
   }

   @Override
   public void visit(int version, int access, String name,
                     String signature, String superName, String[] interfaces) {
      cv.visit(V1_5, access, name, signature, superName, interfaces);
   }
}

public class InstrumentationAgent implements ClassFileTransformer {


   public static void premain(String args, Instrumentation inst) {
      new InstrumentationAgent().run(args, inst);
   }
   
   private void run(String args, Instrumentation inst) {
      System.err.println("Initializing instrumentation ...");
      inst.addTransformer(this);
   }

   @Override
   public byte[] transform(ClassLoader loader, String className,
         Class<?> classBeingRedefined, ProtectionDomain protectionDomain,
         byte[] classfileBuffer) throws IllegalClassFormatException {
      // System.err.println("Transforming " + className);

      // simple sample
      String method = "com.example.HelloWorld.printGreeting()";
      Signature s = Signature.getSignature(method);

      byte[] result = classfileBuffer;
      if (s.matchesClassName(className)) {
         System.err.println("Instrumenting " + className);

         ClassReader cr = new ClassReader(classfileBuffer);
         ClassWriter cw = new ClassWriter(cr, 0);

         ChangeVersionAdapter ca = new ChangeVersionAdapter(cw);
         cr.accept(ca, 0);

         result = cw.toByteArray();
      }

      return result;
   }
   
   
}
