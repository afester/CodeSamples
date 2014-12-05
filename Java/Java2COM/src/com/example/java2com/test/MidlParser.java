package com.example.java2com.test;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

import org.antlr.v4.runtime.ANTLRInputStream;

import com.example.java2com.idl.MIDLLexer;

public class MidlParser {
    public static void main(String[] args) throws FileNotFoundException, IOException {
        
        String projDir = System.getProperty("user.dir");
        String idlTestFile = projDir + File.separator + "visio.idl";
 
        ANTLRInputStream input = new ANTLRInputStream(new FileInputStream(idlTestFile));

        // create a lexer that feeds off of input CharStream
        MIDLLexer lexer = new MIDLLexer(input);

        // create a buffer of tokens pulled from the lexer
        //CommonTokenStream tokens = new CommonTokenStream(lexer);

        for (int i = 0;  i < 2000; i++) {
            System.err.println(lexer.nextToken());
        }

        // create a parser that feeds off the tokens buffer
//        ArrayInitParser parser = new ArrayInitParser(tokens);
//        ParseTree tree = parser.init(); // begin parsing at init rule
//        System.out.println(tree.toStringTree(parser)); // print LISP-style tree
    }
}
