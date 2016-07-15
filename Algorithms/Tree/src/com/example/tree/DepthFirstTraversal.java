package com.example.tree;

public class DepthFirstTraversal<T> implements TreeTraversal<T> {

    @Override
    public void traversePreOrder(TreeNode<T> tree, TreeNodeVisitor<T> visitor) {
        visitor.visit(tree);
        for (TreeNode<T> node : tree.getChildren()) {
            traversePreOrder(node, visitor);
        }
    }

//    @Override
//    public void traverseInOrder(TreeNode<T> tree, TreeNodeVisitor<T> visitor) {
// Only defined for binary trees -
//  traverse(tree.getLeft());
//  visit();
//  traverse(tree.getRight());
//    }

    @Override
    public void traversePostOrder(TreeNode<T> tree, TreeNodeVisitor<T> visitor) {
        for (TreeNode<T> node : tree.getChildren()) {
            traversePostOrder(node, visitor);
        }
        visitor.visit(tree);
    }

}
