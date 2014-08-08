package com.example.asm;

import java.io.IOException;

import org.objectweb.asm.ClassReader;
import org.objectweb.asm.ClassVisitor;
import org.objectweb.asm.MethodVisitor;
import org.objectweb.asm.Type;

import static org.objectweb.asm.Opcodes.*;

class ClassPrinter extends ClassVisitor {

   @Override
   public MethodVisitor visitMethod(int access, String name, String desc,
         String signature, String[] exceptions) {

      StringBuffer methodSignature = new StringBuffer();

      Type t = Type.getType(desc);
      Type returnType = t.getReturnType();

      methodSignature.append(returnType.getClassName());
      methodSignature.append(" ");
      methodSignature.append(name);
      methodSignature.append("(");

      boolean first = true;
      Type[] arguments = t.getArgumentTypes();
      for (Type a : arguments) {
         if (!first) {
            methodSignature.append(", ");
         }
         methodSignature.append(a.getClassName());
         first = false;
      }
     
      methodSignature.append(")");

      System.err.println(methodSignature);

      return super.visitMethod(access, name, desc, signature, exceptions);
   }

   public ClassPrinter() {
      super(ASM4);
   }
   
}

public class ClassDumper {
   
   public void run() {
      
      try {
         ClassReader cr = new ClassReader("java.lang.String");
         ClassPrinter cp = new ClassPrinter();
         cr.accept(cp, 0);
      } catch (IOException e) {
         e.printStackTrace();
      }
      
   }
   
   public static void main(String[] args) {
      new ClassDumper().run();
   }
}
