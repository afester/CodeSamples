package com.example.tree;

public interface TreeTraversal<T> {
    // Visit current node, then traverse children
    void traversePreOrder(TreeNode<T> tree, TreeNodeVisitor<T> visitor);

    // Traverse left child, visit current node, traverse right child
    // (only useful for binary tree)
    default void traverseInOrder(TreeNode<T> tree, TreeNodeVisitor<T> visitor) {
        throw new UnsupportedOperationException();
    }

    // traverse children, then visit current node
    default void traversePostOrder(TreeNode<T> tree, TreeNodeVisitor<T> visitor) {
        throw new UnsupportedOperationException();
    }
}
