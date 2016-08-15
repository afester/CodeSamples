package com.example.tree;

public class TreeNodeCounterVisitor<T> implements TreeNodeVisitor<T> {

    private int count = 0;

    @Override
    public void visit(TreeNode<T> node) {
        count++;
    }

    
    public int getResult() {
        return count;
    }
}
