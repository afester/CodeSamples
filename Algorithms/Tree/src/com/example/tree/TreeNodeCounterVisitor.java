package com.example.tree;

public class TreeNodeCounterVisitor implements TreeNodeVisitor {

    private int count = 0;

    @Override
    public <T> void visit(TreeNode<T> node) {
        count++;
    }

    
    public int getResult() {
        return count;
    }
}
