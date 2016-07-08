package com.example.tree;

import java.util.LinkedList;
import java.util.Queue;

public class BreadthFirstTraversal<T> implements TreeTraversal<T> {

    @SuppressWarnings("rawtypes")
    private Queue<TreeNode> nodeQueue = new LinkedList<>();

    @Override
    public void traverse(TreeNode<T> node, TreeNodeVisitor<T> visitor) {

        nodeQueue.add(node);

        // get next node from the queue, visit it and add its children to the queue
        while(nodeQueue.size() > 0) {
            @SuppressWarnings("unchecked")
            TreeNode<T> n = nodeQueue.poll();

            visitor.visit(n);
            
            for (TreeNode<T> childNode : n.getChildren()) {
                nodeQueue.add(childNode);
            }
        }
    }
}
