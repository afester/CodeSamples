package com.example.java2com.midl;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class MethodDesc {

    @SuppressWarnings("serial")
    static final Set<String> keywords = new HashSet<String>() {{
        add("abstract");  add("assert");    add("boolean");     add("break");  
        add("byte");      add("case");      add("catch");       add("char");  
        add("class");     add("const");     add("continue");    add("default");  
        add("do");        add("double");    add("else");        add("extends");
        add("false");     add("final");     add("finally");     add("float");  
        add("for");       add("goto");      add("if");          add("implements");  
        add("import");    add("instanceof");add("int");         add("interface");
        add("long");      add("native");    add("new");         add("null");        
        add("package");   add("private");   add("protected");   add("public");
        add("return");    add("short");     add("static");      add("strictfp");  
        add("super");     add("switch");    add("synchronized");add("this"); 
        add("throw");     add("throws");    add("transient");   add("true");     
        add("try");       add("void");      add("volatile");    add("while");
    }};

    private String name;
    public Map<String, AnnotationDesc> annotations = new HashMap<>();
    private List<ParameterDesc> parameters = new ArrayList<>();

    public MethodDesc(String mName) {
        this.name = mName;
    }

    public void addParameter(ParameterDesc currentParameter) {
        parameters.add(currentParameter);
    }

    public String getName() {
        return name;
    }

    public List<ParameterDesc> getParameters() {
        return parameters;
    }

    public Map<String, AnnotationDesc> getAnnotations() {
        return annotations;
    }

/*Convenience methods */    

    public boolean isPut() {
        return annotations.containsKey("propput");
    }

    public boolean isGet() {
        return annotations.containsKey("propget");
    }

    public String getJavaName() {
        String result = name;
        String prefix = "";
        if (isPut()) {
            prefix = "set";
        } else if (isGet()) {
            prefix = "get";
        } else {
            String first = result.substring(0, 1);
            String last = result.substring(1);
            result = first.toLowerCase() + last;

            if (keywords.contains(result)) {
                result = "_" + result;
            }
        }

        return prefix + result;
    }

    public TypeDesc getReturnType() {
        for (ParameterDesc param : parameters) {
            if (param.isReturn()) {
                return param.getType(); // new TypeDesc(param.getTypeName());
            }
        }

        return null; // void
    }
}
