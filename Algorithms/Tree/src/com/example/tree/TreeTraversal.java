package com.example.tree;

public abstract class TreeTraversal {

    public abstract <T> void traverse(TreeNode<T> tree, TreeNodeVisitor visitor);
}
