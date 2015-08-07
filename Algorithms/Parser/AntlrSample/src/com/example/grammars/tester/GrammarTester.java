package com.example.grammars.tester;

import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import org.antlr.v4.runtime.ANTLRInputStream;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.Parser;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.tree.ParseTree;
import org.antlr.v4.runtime.tree.TerminalNode;
import org.antlr.v4.runtime.tree.TerminalNodeImpl;

public class GrammarTester {

    private String inputFile;
    private String lexerClassname;
    private String parserClassname;
    private String startRule;

    public GrammarTester(String grammarFile, String startRule, String inputFile) {
        
        String pathName = "";
        String fileName = grammarFile;

        int idx = grammarFile.lastIndexOf('/');
        if (idx >= 0) {
            pathName = grammarFile.substring(0,  idx);
            fileName = grammarFile.substring(idx + 1);
        }
        this.inputFile = pathName + '/' + inputFile;

        String grammarName = fileName;
        idx = fileName.lastIndexOf('.');
        if (idx >= 0) {
            grammarName = fileName.substring(0, idx);
        }

        if (pathName.startsWith("/")) {
            pathName = pathName.substring(1);
        }
        String packageName = pathName.replace('/',  '.');

        this.lexerClassname = packageName + '.' + grammarName + "Lexer";
        this.parserClassname = packageName + '.' + grammarName + "Parser";
        this.startRule = startRule;
    }

    private void dumpTokens() {
        System.err.println("Token Stream:");
        System.err.println("=======================");

        InputStream is = getClass().getResourceAsStream(inputFile);
        try {
            ANTLRInputStream input = new ANTLRInputStream(is);
            Lexer lexer = createLexer(input);
            while(true) {
               Token t = lexer.nextToken();
               if (t.getType() == Token.EOF) {
                  break;
               }

               System.err.println("  " + t);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

    }

    private void dumpParseTree() {
        System.err.println("\nParse Tree:");
        System.err.println("=======================");

        ParseTree tree = parse();

        System.err.println(tree.toStringTree());
    }
    
    private String indent(int level) {
        if (level == 0) {
            return "";
        }

        return String.format("%" + level + "s", "");
    }

    
    private void renderParseTree(ParseTree node, int level) {
        boolean isTerminal = false;
        if (node instanceof TerminalNode) {
            isTerminal = true;
        }

        if (isTerminal) {
            TerminalNodeImpl tni = (TerminalNodeImpl) node;
            System.err.printf("%s \"%s\"\n", indent(level*3), node.getText());
        } else {
            String context = node.getClass().getSimpleName();
            if (context.endsWith("Context")) {
                context = context.substring(0, context.length() - "Context".length());
            }
            System.err.printf("%s %s\n", indent(level*3), context);
        }

        for (int idx = 0;   idx < node.getChildCount();  idx++) {
            ParseTree child = node.getChild(idx);
            renderParseTree(child, level + 1);
        }
    }

    
    private void renderParseTree() {
        System.err.println("\nParse Tree:");
        System.err.println("=======================");

        ParseTree tree = parse();
        renderParseTree(tree, 0);
    }

    
    private ParseTree parse() {
        InputStream is = getClass().getResourceAsStream(inputFile);

        ANTLRInputStream input = null;
        try {
            input = new ANTLRInputStream(is);
        } catch (IOException e) {
            e.printStackTrace();
        }
        Lexer lexer = createLexer(input);
        
        CommonTokenStream tokens = new CommonTokenStream(lexer);
        Parser parser = createParser(tokens);

        ParseTree tree = invokeStartMethod(parser);
        return tree;
    }

    
    
    private ParseTree invokeStartMethod(Parser parser) {
        ParseTree tree = null;

        try {
            Method start = parser.getClass().getMethod(startRule);
            tree = (ParseTree) start.invoke(parser);
        } catch (NoSuchMethodException | SecurityException | 
                 IllegalAccessException | IllegalArgumentException | 
                 InvocationTargetException e) {
            e.printStackTrace();
        }

        return tree;
    }

    
    private Parser createParser(CommonTokenStream tokens) {
        Parser parser = null;

        try {
            Class<?> parserClass = Class.forName(parserClassname);
            Constructor<?> cons = parserClass.getConstructor(TokenStream.class);
            parser = (Parser) cons.newInstance(tokens);
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (SecurityException e) {
            e.printStackTrace();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }

        return parser;
    }

    
    private Lexer createLexer(ANTLRInputStream input) {
        Lexer lexer = null;

        try {
            Class<?> lexerClass = Class.forName(lexerClassname);
            Constructor<?> cons = lexerClass.getConstructor(CharStream.class);
            lexer = (Lexer) cons.newInstance(input);
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (SecurityException e) {
            e.printStackTrace();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }

        return lexer;
    }

    
    public void run() {
        System.err.printf("Input file: %s\n", inputFile);
        System.err.printf("Lexer     : %s\n", lexerClassname);
        System.err.printf("Parser    : %s\n\n", parserClassname);

//        dumpTokens();
//        dumpParseTree();
        renderParseTree();
    }

    public static void main(String[] args) {
        //final String grammar = "/com/example/grammars/arrayinit/ArrayInit.g4";
        //final String startRule = "init";
        //final String inputFile = "ArrayInit.txt";

        //final String grammar = "/com/example/grammars/expr/Expr.g4";
        //final String startRule = "prog";
        //final String inputFile = "Expr.txt";

        //final String grammar = "/com/example/grammars/uuids/UUIDs.g4";
        //final String startRule = "uuidlist";
        //final String inputFile = "UUIDs.txt";

        final String grammar = "/com/example/grammars/midl/Midl.g4";
        final String startRule = "midl";
        final String inputFile = "Midl.txt";

        new GrammarTester(grammar, startRule, inputFile).run();
    }

}
