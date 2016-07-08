package com.example.tree;

@FunctionalInterface
public interface TreeTraversal<T> {
    void traverse(TreeNode<T> tree, TreeNodeVisitor<T> visitor);
}
