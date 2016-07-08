package com.example.tree;

@FunctionalInterface
public interface TreeNodeVisitor<T> {
    void visit(TreeNode<T> node); 
}
