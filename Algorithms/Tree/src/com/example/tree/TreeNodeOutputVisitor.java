package com.example.tree;

public class TreeNodeOutputVisitor<T> implements TreeNodeVisitor<T> {

    @Override
    public void visit(TreeNode<T> node) {
        System.err.println("Operating on " + node);
    }
}
