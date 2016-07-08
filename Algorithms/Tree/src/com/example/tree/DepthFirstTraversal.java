package com.example.tree;

public class DepthFirstTraversal<T> implements TreeTraversal<T> {

    @Override
    public void traverse(TreeNode<T> tree, TreeNodeVisitor<T> visitor) {
        visitor.visit(tree);
        for (TreeNode<T> node : tree.getChildren()) {
            traverse(node, visitor);
        }
    }

}
