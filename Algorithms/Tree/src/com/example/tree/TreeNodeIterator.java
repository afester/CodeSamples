package com.example.tree;

import java.util.ArrayList;
import java.util.List;

/**
 * Note: this iterator supports wrap around from last to first and from 
 * first to last!
 *
 */
public class TreeNodeIterator {

    private List<TreeNode> nodes = new ArrayList<>();
    private int idx = -1;

    public TreeNodeIterator(TreeNode<String> tree) {
        // most simple approach: collect all nodes into a list and use an index
        // into the list
        
        TreeTraversal<String> tt1 = new DepthFirstTraversal();
        tt1.traverse(tree, new TreeNodeVisitor<String>() {

                @Override
                public void visit(TreeNode<String> node) {
                    nodes.add(node);
                }
            } );
    }

    public <T> TreeNode<T> next() {
        if (idx == -1) {
            idx = 0;
        } else {
            idx++;
            if (idx >= nodes.size()) {
                idx = 0;
            }
        }

        return nodes.get(idx);
    }

    public <T> TreeNode<T> prev() {
        if (idx == -1) {
            idx = nodes.size() - 1;
        } else {
            idx--;
            if (idx < 0)  {
                idx = nodes.size() - 1;
            }
        }

        return nodes.get(idx);
    }
}
