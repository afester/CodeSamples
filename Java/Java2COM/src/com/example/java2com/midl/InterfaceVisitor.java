package com.example.java2com.midl;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.antlr.v4.runtime.ANTLRInputStream;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.tree.ParseTree;

import com.example.java2com.midl.grammar.MidlBaseVisitor;
import com.example.java2com.midl.grammar.MidlLexer;
import com.example.java2com.midl.grammar.MidlParser;
import com.example.java2com.midl.grammar.MidlParser.EnumElementContext;
import com.example.java2com.midl.grammar.MidlParser.INTERFACEContext;
import com.example.java2com.midl.grammar.MidlParser.MethodContext;
import com.example.java2com.midl.grammar.MidlParser.ParameterContext;
import com.example.java2com.midl.grammar.MidlParser.PropertiesContext;
import com.example.java2com.midl.grammar.MidlParser.PropertyContext;
import com.example.java2com.midl.grammar.MidlParser.PropparamContext;
import com.example.java2com.midl.grammar.MidlParser.TYPEDEFContext;

class Parameter {
   public String name;
   public String type;
}

class MyVisitor extends MidlBaseVisitor {


    private MidlDesc midl = new MidlDesc();
    private InterfaceDesc currentInterface = null;
    private MethodDesc currentMethod = null;
    private ParameterDesc currentParameter = null;
    private Map<String, AnnotationDesc> currentAnnotations = null;


    // occurs in the context of Interface, Method and Parameter!
    @Override
    public Object visitProperty(PropertyContext ctx) {
        if (currentAnnotations != null) {
            String annotationName = ctx.ID().getText();
            List<String> annotationParams = new ArrayList<>();
            for (PropparamContext param : ctx.propparam()) {
                annotationParams.add(param.getText());
            }
            
            // System.err.println("       ANNOTATION:" + annotationName + "=" + annotationParams);
            currentAnnotations.put(annotationName, new AnnotationDesc(annotationName, annotationParams));
            
        }

        return super.visitProperty(ctx);
    }


    @Override
    public Object visitProperties(PropertiesContext ctx) {
        Object result = super.visitProperties(ctx);
        return result;
    }

/*****************************************************************************/

    @Override
    public Object visitParameter(ParameterContext ctx) {
        if (currentMethod != null) {

            //TypeContext pType = ctx.type();
            //pType.simpleType(); //!!!!

            TypeDesc td = new TypeDesc(ctx.type().simpleType().ID().getText());

            String pName = ctx.ID().getText();
            currentParameter = new ParameterDesc(pName, td); 
            currentAnnotations = currentParameter.annotations;  // TODO
        }

        Object result = super.visitParameter(ctx);

        if (currentMethod != null) {
        currentMethod.addParameter(currentParameter);
        currentParameter = null; 
        currentAnnotations = null;

        //System.err.println("  ---PARAMETER");
        }

        return result;
    }


    @Override
    public Object visitMethod(MethodContext method) {
        if (currentInterface != null) {
    
            String mName = null;
            if (method.ID().size() == 2) {
                // mModifier =  method.ID().get(0).getText();
                mName = method.ID().get(1).getText();
            } else {
                mName = method.ID().get(0).getText();
            }
            // System.err.println("  METHOD:" + mName);
            currentMethod = new MethodDesc(mName);
            currentAnnotations = currentMethod.annotations;  // TODO
        }

        Object result = super.visitMethod(method);

        if (currentInterface != null) {
            currentInterface.addMethod(currentMethod);
            currentMethod = null;
            currentAnnotations = null;
    
           // System.err.println("  ---METHOD");
        }

        return result;
    }

    @Override
    public Object visitINTERFACE(INTERFACEContext ctx) {
        String name = ctx.ID().get(0).getText();
        String superName = ctx.ID().get(1).getText();
        
        // System.err.println("INTERFACE:" + name);
        
        currentInterface = new InterfaceDesc(name, superName);
        currentAnnotations = currentInterface.annotations;  // TODO

        Object result = super.visitINTERFACE(ctx);

        // System.err.println("---INTERFACE");
        midl.addInterface(currentInterface);
        currentInterface = null;

        return result;
    }

    @Override
    public Object visitTYPEDEF(TYPEDEFContext ctx) {

        // currently only supporting ENUM types
        if (ctx.type().enumElement().size() > 0) {

            String enumName = ctx.ID().getText();
            EnumDesc enumDesc = new EnumDesc(enumName);
            for (EnumElementContext eec : ctx.type().enumElement()) {
                String elementName = eec.ID().getText();
                String elementValue = "0";
                if (eec.NUMBER() != null) {
                    elementValue = eec.NUMBER().getText();
                } else if (eec.HEXNUMBER() != null) {
                    elementValue = eec.HEXNUMBER().getText();
                }
                enumDesc.addElement(new EnumElement(elementName, elementValue));
            }

            midl.addEnum(enumDesc);
        }

        Object result = super.visitTYPEDEF(ctx);

        return result;
    }


    public void dumpResult() {
        System.err.println("RESULT:\n---------------------------");
        for (InterfaceDesc id : midl.getInterfaces()) {
            System.err.printf("  %s : %s %s\n", id.getName(), id.getSuperName(), id.getAnnotations() );
            for (MethodDesc md : id.getMethods()) {
                System.err.println("    " + md.getName() + md.getAnnotations());

                for (ParameterDesc pd : md.getParameters()) {
                    System.err.println("      " + pd.getName() + pd.getAnnotations());
                }
            }
        }
    }


    public void dumpJava() {
        String destFolder = "target/generated-sources/midl/";
        String packageName = "com.example.java2com.excel";

/*********************/        
        destFolder = destFolder +  packageName.replace('.',  '/') + "/";
        File pathCreator = new File(destFolder);
        pathCreator.mkdirs();

        for (EnumDesc ed : midl.getEnums()) {
            System.err.printf("Generating %s ...\n", ed.getJavaFileName());
            try (PrintWriter pw = new PrintWriter(destFolder + ed.getJavaFileName())) {
                pw.printf("package %s;\n\n", packageName);

                pw.printf("public enum %s {\n\n", ed.getName());

                pw.printf("}");
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            }
        }

        for (InterfaceDesc id : midl.getInterfaces()) {

            
            // for now, only support IDispatch base class
            if ("IDispatch".equals(id.getSuperName())) {
                
                System.err.printf("Generating %s ...\n", id.getJavaFileName());
    
                try (PrintWriter pw = new PrintWriter(destFolder + id.getJavaFileName())) {
                    pw.printf("package %s;\n\n", packageName);

                    if (id.isIterable()) {
                        pw.printf("import java.util.Iterator;\n");
                    }
                    pw.printf("import com.example.java2com.COMProxy;\n");
                    pw.printf("import com.example.java2com.IUnknown;\n");
                    pw.printf("import com.example.java2com.IDispatch;\n");
                    pw.printf("import com.example.java2com.Variant;\n");
                    pw.printf("import com.example.java2com.VariantOut;\n\n");

                    pw.printf("/**\n");
                    if (id.getHelpstring() != null) {
                        pw.printf(" * %s\n", id.getHelpstring());   
                    }
                    if (id.getUUID() != null) {
                        pw.printf(" * uuid=%s\n", id.getUUID());   
                    }
                    pw.printf(" */\n");
                    
                    String extendsAndImplements = "extends " + id.getSuperName();
                    if (id.isIterable()) {
                        String retType = id.getMethod("Item").getReturnType().getName();
                        extendsAndImplements = extendsAndImplements + " implements Iterable<" + retType + ">";
                    }

                    pw.printf("public class %s %s {\n\n", id.getName(), extendsAndImplements);
    
                    pw.printf("  public %s(COMProxy ci) {\n"+
                              "    super(ci);\n"+
                              "  }\n\n", id.getName());

                    if (id.isIterable()) {
                        
                        String retType = id.getMethod("Item").getReturnType().getName();
                        String idxType = id.getMethod("Count").getReturnType().getName();
                        String idxParam = "";
                        if (id.getMethod("Item").getParameters().get(0).getTypeName().equals("Variant")) {
                            idxParam = "new Variant(idx++)";
                        } else {
                            idxParam = "idx++";
                        }
                        pw.printf("  public Iterator<%s> iterator() {\n"+
                                  "    return new Iterator<%s>() {\n"+
                                  "\n"+
                                  "      %s idx = 1;\n"+
                                  "      %s count = getCount();\n"+
                                  "\n"+
                                  "      @Override\n"+
                                  "      public boolean hasNext() {\n"+
                                  "        return idx <= count;\n"+
                                  "      }\n"+
                                  "\n"+
                                  "      @Override\n"+
                                  "      public %s next() {\n"+
                                  "        return getItem(%s);\n"+
                                  "      }\n"+
                                  "\n"+
                                  "    };\n"+
                                  "  }\n\n", retType, retType, idxType, idxType, retType, idxParam);
                    }

                    for (MethodDesc md : id.getMethods()) {
    
                        String params = "";
                        for (ParameterDesc pd : md.getParameters()) {
                            if (!pd.isReturn()) {
                                if (!params.isEmpty()) {
                                    params = params + ", ";
                                }

                                if (pd.isOut()) {
                                    params = params + "VariantOut " + pd.getName();
                                } else {
                                    params = params + pd.getTypeName() + " " + pd.getName();
                                }
                            }
                        }

                        TypeDesc returnType = md.getReturnType();
                        String retTypeName = "void";
                        if (returnType != null) {
                            retTypeName = returnType.getName();
                        }

                        pw.printf("  public %s %s(%s){\n", retTypeName, md.getJavaName(), params);

/**************************************************/

                        String action = "invoke";
                        if (md.isGet()) {
                            action = "getProperty";
                        } else if (md.isPut()) {
                            action = "setProperty";
                        }

                        boolean uoe = false;
                        String reason = "";
                        String invokeParams = "";
                        for (ParameterDesc pd : md.getParameters()) {
                            if (!pd.isReturn()) {

                                if (pd.isOut()) {

                                    invokeParams = invokeParams + ", " + pd.getName();

                                } else {
                                    
                                    // already a variant, no wrapper necessary
                                    if (pd.getTypeName().equals("Variant") ) {
                                        invokeParams = invokeParams + 
                                                ", " + pd.getName();
    
                                    // known interface or primitive type
                                    } else if(pd.getTypeName().equals("String") ||
                                              pd.getTypeName().equals("long") ||
                                              pd.getTypeName().equals("int") ||
                                              pd.getTypeName().equals("double") ||
                                              midl.getInterface(pd.getTypeName()) != null) {
    
                                        invokeParams = invokeParams + 
                                                       ", new Variant(" + pd.getName() + ")";
                                    } else {   /* TODO: Support enums */
    
                                        uoe = true;
                                        reason = "Parameter type: " + pd.getTypeName();
                                        break;
    
                                    }
                                }
                            }
                        }

                        if (uoe == false) {
                            if (returnType == null) {
                                pw.printf("    ci.%s(\"%s\"%s);\n", action, md.getName(), invokeParams);
                            } else {
                                emitCallWithResult(action, invokeParams, pw, md);
                            }
                        } else {
                            pw.printf("    throw new UnsupportedOperationException(\"%s\");\n", reason);
                        }

/**************************************************/

                        pw.printf("  }\n\n");
                    }
    
                    pw.printf("}");
                } catch (FileNotFoundException e) {
                    e.printStackTrace();
                }
            } else {
                System.err.printf("Skipping %s ...\n", id.getJavaFileName());
            }
        }
    }


    private void emitCallWithResult(String action, String params, PrintWriter pw, MethodDesc md) {
        
        TypeDesc returnType = md.getReturnType();
        
        if (returnType.getName().equals("String")) {
            pw.printf("    return ci.%s(\"%s\"%s).strValue;\n",
                      action, md.getName(), params);
        } else if (returnType.getName().equals("boolean")) {
            pw.printf("    return ci.%s(\"%s\"%s).booleanValue;\n",
                    action, md.getName(), params);
        } else if (returnType.getName().equals("int")) {
            pw.printf("    return ci.%s(\"%s\"%s).intValue;\n",
                    action, md.getName(), params);
        } else if (returnType.getName().equals("char")) {
            pw.printf("    throw new UnsupportedOperationException(\"Return type char\");\n");
        } else if (returnType.getName().equals("long")) {
            pw.printf("    return ci.%s(\"%s\"%s).intValue;\n",
                    action, md.getName(), params);
        } else if (returnType.getName().equals("double")) {
            pw.printf("    return ci.%s(\"%s\"%s).doubleValue;\n",
                    action, md.getName(), params);
        } else if (returnType.getName().equals("Variant")) {
            pw.printf("    return ci.%s(\"%s\"%s);\n",
                    action, md.getName(), params);
        } else if (/* check if type is known */
                   midl.getInterface(returnType.getName()) == null ||

                   /* TODO: Support enums */
                   midl.getEnum(returnType.getName()) != null) {

            pw.printf("    throw new UnsupportedOperationException(\"Return type\");\n");

        } else {
            pw.printf("    return new %s(ci.%s(\"%s\"%s).dispatch);\n",
                      md.getReturnType().getName(), action, md.getName(), params);
        }
    }

}


public class InterfaceVisitor {

    public void run() {
        try (InputStream is = new FileInputStream("Excel.idl")) {
            ANTLRInputStream input = null;
            try {
                input = new ANTLRInputStream(is);
            } catch (IOException e) {
                e.printStackTrace();
            }
    
            Lexer lexer = new MidlLexer(input);
    
            CommonTokenStream tokens = new CommonTokenStream(lexer);
    
            MidlParser p = new MidlParser(tokens);
            ParseTree t = p.midl();
    
            MyVisitor v = new MyVisitor();
            v.visit(t);
    
            v.dumpJava();
        } catch (FileNotFoundException e1) {
            e1.printStackTrace();
        } catch (IOException e2) {
            e2.printStackTrace();
        }

        // System.err.println(t.toStringTree());
    }


    public static void main(String[] args) {
        new InterfaceVisitor().run();
    }
    
}
