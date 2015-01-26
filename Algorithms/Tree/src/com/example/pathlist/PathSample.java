package com.example.pathlist;

import java.util.ArrayList;
import java.util.List;

import com.example.tree.TreeAsciiRenderer;
import com.example.tree.TreeNode;


public class PathSample {
    
    
    
    private void run() {
        Path p = new Path();
        p.append("A")
         .append("B")
         .append("C");
        System.err.println(p);
        System.err.println(p.asString("/"));
        
        System.err.println();
        Path p2 = new Path("A", "B", "C");
        System.err.println(p2);
        System.err.println(p2.asString("/"));

        System.err.println();
        List<Path> paths = new ArrayList<Path>();
        paths.add(new Path("A", "B"));
        paths.add(new Path("A", "B", "C", "D"));
        paths.add(new Path("A", "E"));
        paths.add(new Path("Z", "Y", "X"));
        paths.add(new Path("A", "E", "F", "G"));
        paths.add(new Path("A", "H", "I"));
        for (Path p3 : paths) {
            System.err.println(p3.asString("/"));
        }

        TreeNode<String> root = createTreeFromPaths(paths);

        // render all trees which have a common root node
        System.err.println();
        TreeAsciiRenderer tv = new TreeAsciiRenderer(System.err);
        for (TreeNode<String> child : root.getChildren()) {
          tv.renderHierarchical(child);
        }

    }


    private TreeNode<String> createTreeFromPaths(List<Path> paths) {
        TreeNode<String> result = new TreeNode<>("");

        // add all paths to the tree
        for (Path p : paths) {
            TreeNode<String> current = result;

            for (String e : p) {
                TreeNode<String> n = current.getChild(e);
                if (n == null) {
                    n = new TreeNode<>(e);
                    current.addChildren(n);
                } 
                current = n;
            }
        }

        return result;
    }


    public static void main(String[] args) {
       new PathSample().run();
    }

}
