package com.example.tree;

public class TreeNodeOutputVisitor implements TreeNodeVisitor {

    @Override
    public <T> void visit(TreeNode<T> node) {
        System.err.println("Operating on " + node);
    }
}
