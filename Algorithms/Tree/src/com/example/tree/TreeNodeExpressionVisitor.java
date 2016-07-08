package com.example.tree;

public class TreeNodeExpressionVisitor<T> implements TreeNodeVisitor<T> {

    @Override
    public void visit(TreeNode<T> node) {
        System.err.println(node);
    }

}
