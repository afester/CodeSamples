package com.example.tree;

public class TreeNodeExpressionVisitor implements TreeNodeVisitor {

    @Override
    public <T> void visit(TreeNode<T> node) {
        System.err.println(node);
    }

}
