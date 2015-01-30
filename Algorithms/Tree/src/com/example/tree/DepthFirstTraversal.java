package com.example.tree;

public class DepthFirstTraversal extends TreeTraversal {

    @Override
    public <T> void traverse(TreeNode<T> tree, TreeNodeVisitor visitor) {
        visitor.visit(tree);
        for (TreeNode<T> node : tree.getChildren()) {
            traverse(node, visitor);
        }
    }

}
