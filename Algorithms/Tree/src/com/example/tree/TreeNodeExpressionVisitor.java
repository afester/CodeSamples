package com.example.tree;

public class TreeNodeExpressionVisitor extends TreeNodeVisitor {

    @Override
    public <T> void visit(TreeNode<T> node) {
        System.err.println(node);
    }

}
