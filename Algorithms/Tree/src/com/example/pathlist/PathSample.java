package com.example.pathlist;

import java.util.ArrayList;
import java.util.List;


public class PathSample {
    
    
    
    private void run() {
        Path p = new Path();
        p.append("A")
         .append("B")
         .append("C");
        System.err.println(p);
        System.err.println(p.asString("/"));
        
        Path p2 = new Path("A", "B", "C");
        System.err.println(p2);
        System.err.println(p2.asString("/"));

        System.err.println();
        List<Path> tree = new ArrayList<Path>();
        tree.add(new Path("A", "B"));
        tree.add(new Path("A", "B", "C", "D"));
        tree.add(new Path("A", "E"));
        tree.add(new Path("A", "E", "F", "G"));
        tree.add(new Path("A", "H", "I"));
        for (Path p3 : tree) {
            System.err.println(p3.asString("/"));
        }
    }


    public static void main(String[] args) {
       new PathSample().run();
    }

}
