package com.example.tree;

public abstract class TreeNodeVisitor {

    public abstract <T> void visit(TreeNode<T> node); 
}
