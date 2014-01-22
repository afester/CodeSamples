package com.example;

import org.antlr.v4.runtime.tree.ParseTree;
import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.FieldVisitor;
import org.objectweb.asm.Label;
import org.objectweb.asm.MethodVisitor;
import org.objectweb.asm.Opcodes;

import com.example.expressionparser.ExpressionBaseVisitor;
import com.example.expressionparser.ExpressionParser;
import com.example.expressionparser.ExpressionParser.AddSubContext;
import com.example.expressionparser.ExpressionParser.FunctionContext;
import com.example.expressionparser.ExpressionParser.IdContext;
import com.example.expressionparser.ExpressionParser.MulDivContext;
import com.example.expressionparser.ExpressionParser.NumberContext;
import com.example.expressionparser.ExpressionParser.ParContext;
import com.example.expressionparser.ExpressionParser.PowerContext;

import com.example.symboltable.Applyable;
import com.example.symboltable.Constant;
import com.example.symboltable.CosFunction;
import com.example.symboltable.Function;
import com.example.symboltable.LogFunction;
import com.example.symboltable.NoFunctionException;
import com.example.symboltable.SinFunction;
import com.example.symboltable.SqrtFunction;
import com.example.symboltable.Symbol;
import com.example.symboltable.SymbolTable;
import com.example.symboltable.TanFunction;
import com.example.symboltable.UndefinedSymbolException;
import com.example.symboltable.Variable;

import static org.objectweb.asm.Opcodes.*;


class ExpressionClassLoader extends ClassLoader {
   public Class<?> defineClass(String name, byte[] b) {
      return defineClass(name, b, 0, b.length);
   }
}


public class ExpressionCompiler extends ExpressionBaseVisitor<Void> {

   private SymbolTable symbolTable = new SymbolTable();
   private ClassWriter cw = new ClassWriter(ClassWriter.COMPUTE_MAXS);
   private MethodVisitor mv = null;

   
   public ExpressionCompiler() {
      symbolTable.putSymbol(new Constant("PI", Math.PI));
      symbolTable.putSymbol(new Constant("e", Math.E));

      symbolTable.putSymbol(new SinFunction());
      symbolTable.putSymbol(new CosFunction());
      symbolTable.putSymbol(new TanFunction());
      symbolTable.putSymbol(new LogFunction());
      symbolTable.putSymbol(new SqrtFunction());
   }


   public Expression compile(ParseTree tree) {

      cw.visit(V1_7, 
               ACC_SUPER + ACC_PUBLIC,
               "com/example/CompiledExpr",      // class name 
               null,                            // no generics
               "com/example/AbstractExpression",// super type
               null);                           // no interfaces

      {/***** Constructor ********************/
         mv = cw.visitMethod(ACC_PUBLIC, "<init>", "()V", null, null);
         mv.visitCode();

         Label l0 = new Label();
         mv.visitLabel(l0);

         mv.visitVarInsn(ALOAD, 0);
         mv.visitMethodInsn(INVOKESPECIAL, "com/example/AbstractExpression", "<init>", "()V");
         mv.visitInsn(RETURN);

         Label l1 = new Label();
         mv.visitLabel(l1);

         mv.visitLocalVariable("this", "Lcom/example/CompiledExpr;", null, l0, l1, 0);
         mv.visitMaxs(0, 0);
         mv.visitEnd(); 
      }

      {/***** evaluate() method ******************/
         mv = cw.visitMethod(ACC_PUBLIC,           // public 
                             "evaluate", "(D)D",   // double evaluate(double)
                             null,                 // not a generic method
                             null);                // does not throw exceptions
         mv.visitCode();

         Label l0 = new Label();
         mv.visitLabel(l0);

         visit(tree);

         mv.visitInsn(DRETURN);
         Label l1 = new Label();
         mv.visitLabel(l1);
   
         // local variable with index 0 is "this"
         mv.visitLocalVariable("this", "Lcom/example/CompiledExpr;", 
                               null, l0, l1, 0);
   
         // local variable with index 1 is method's parameter (x) 
         mv.visitLocalVariable("x", "D", null, l0, l1, 1);

         // generate additional fields
         symbolTable.apply(new Applyable<Symbol>() {
   
            @Override
            public void each(Symbol s) {
               
               if (s instanceof Variable && !s.getName().equals("x")) {
                  FieldVisitor fv = 
                        cw.visitField(
                              ACC_PUBLIC, s.getName(), 
                              "D", null, 
                              null); // This parameter is only used for static fields. 
                                     // Its value is ignored for non static fields, 
                                     // which must be initialized through bytecode 
                                     // instructions in constructors or methods.
                  fv.visitEnd();
               }
            }
   
         });

         mv.visitMaxs(0, 0);     // values autocalculated
         mv.visitEnd();          // close the method
      }

      cw.visitEnd();

      // create the Class
      byte[] b = cw.toByteArray();

      // TODO: add flag for debug mode
//      PrintWriter pw = new PrintWriter(System.err);

      // dump the generated class from the byte code
//      ClassReader cr = new ClassReader(b);
//      TraceClassVisitor cv2 = new TraceClassVisitor(pw);
//      cr.accept(cv2, 0);

      // verify the created class
//      CheckClassAdapter.verify(cr, true, pw);

      // define the class
      ExpressionClassLoader cl = new ExpressionClassLoader();
      Class<?> c = cl.defineClass("com.example.CompiledExpr", b);

      // create an instance of the class
      try {
         return (Expression) c.newInstance();
      } catch (InstantiationException | IllegalAccessException e) {
         e.printStackTrace();
      }

      return null;
   }


   @Override
   public Void visitFunction(FunctionContext ctx) {

      // get the function and check if it is valid
      String functionName = ctx.ID().getText();

      Symbol fn = symbolTable.getSymbol(functionName);
      if (fn == null) {
         throw new UndefinedSymbolException(functionName);
      }
      if (!(fn instanceof Function)) {
         throw new NoFunctionException(functionName);
      }

      // evaluate the function argument
      visit(ctx.expr());

      mv.visitMethodInsn(INVOKESTATIC, "java/lang/Math", functionName, "(D)D");

      return null;
   }

   @Override
   public Void visitPar(ParContext ctx) {
      visit(ctx.expr());
      return null;
   }

   @Override
   public Void visitPower(PowerContext ctx) {
      visit(ctx.expr(0)); // left tree
      visit(ctx.expr(1)); // right tree

      mv.visitMethodInsn(INVOKESTATIC, "java/lang/Math", "pow", "(DD)D");

      return null;
   }


   @Override
   public Void visitMulDiv(MulDivContext ctx) {
      visit(ctx.expr(0)); // left tree
      visit(ctx.expr(1)); // right tree

      if (ctx.op.getType() == ExpressionParser.MUL) {
         mv.visitInsn(DMUL);
         return null;
      }

      mv.visitInsn(DDIV);
      return null;
   }


   @Override
   public Void visitAddSub(AddSubContext ctx) {
      visit(ctx.expr(0)); // left tree
      visit(ctx.expr(1)); // right tree

      if (ctx.op.getType() == ExpressionParser.ADD) {
         mv.visitInsn(DADD);
         return null;
      }

      mv.visitInsn(DSUB);
      return null;
   }


   @Override
   public Void visitId(IdContext ctx) {
      // get the ID
      String idName = ctx.ID().getText();

      // get the symbol from the symbol table
      Symbol id = symbolTable.getSymbol(idName);
      if (id == null) {
         id = new Variable(idName);
         symbolTable.putSymbol(id);
      }

      if (idName.equals("x")) {
         // push method's argument onto the stack - "x" is passed as
         // the function parameter to "evaluate"
         mv.visitVarInsn(Opcodes.DLOAD,  1);  
      } else {
         
         if (id instanceof Variable) {
            // push field value onto the stack
            mv.visitIntInsn(Opcodes.ALOAD,  0);
            mv.visitFieldInsn(Opcodes.GETFIELD, "com/example/CompiledExpr", idName, "D");
         } else {
            // push constant
            mv.visitLdcInsn(id.getValue());
         }
      }

      return null;
   }


   @Override
   public Void visitNumber(NumberContext ctx) {
      // convert token into Double
      double result = Double.parseDouble(ctx.getText());

      mv.visitLdcInsn(result);

      return null;
   }

/*
   @Override
   public Void visitInt(IntContext ctx) {
      // convert token into Integer
      Double result = Double.parseDouble(ctx.getText());

      mv.visitLdcInsn(result);

      return null;
   }
*/

   public void setVariable(String name, Double value) {
      Variable xVar = (Variable) symbolTable.getSymbol(name);
      if (xVar == null) {
         xVar = new Variable(name);
         symbolTable.putSymbol(xVar);
      }
      xVar.setValue(value);
   }
}
