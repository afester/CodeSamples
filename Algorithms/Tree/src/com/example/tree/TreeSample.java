package com.example.tree;

import java.util.ArrayList;

public class TreeSample {
    
    
    
    public static void main(String[] args) {
        new TreeSample().run();
    }


    private void createSubTree(TreeNode<String> parent, int childCount, int levelCount, String label) {
       for (int cCount = 1;  cCount <= childCount;  cCount++) {
          String newLabel = label + '.' + cCount;
          TreeNode<String> child = new TreeNode<>(newLabel);
          parent.addChildren(child);
          if (levelCount > 1) {
             createSubTree(child, childCount, levelCount - 1, newLabel);
          }
       }
    }


    /**
     *  Creates a tree of a specific size.
     *
     * @param childCount The number of children each node has
     * @param depth      The number of levels of the tree
     *
     * @return A TreeNode as the root node of the tree
     */
    private TreeNode<String> createTree(int childCount, int depth) {
       TreeNode<String> root = new TreeNode<>("Root");
       createSubTree(root, childCount, depth, "Node");
       return root;
    }

    @SuppressWarnings("serial")
    private void run() {
        // Create a sample tree
        TreeNode<String> tree = createTree(3, 3);

        TreeNode<String> addtlNode = tree.findNode(new ArrayList<TreeNode<String>>() {{
                                                     add(new TreeNode<String>("Node.2"));
                                                     add(new TreeNode<String>("Node.2.1"));
                                                     add(new TreeNode<String>("Node.2.1.2"));
                                                   }});
        addtlNode.addChildren(new TreeNode<>("Node.2.1.2.1"));
        addtlNode.addChildren(new TreeNode<>("Node.2.1.2.2"));
        addtlNode.addChildren(new TreeNode<>("Node.2.1.2.3"));

        addtlNode = tree.findNode(new ArrayList<TreeNode<String>>() {{
                                    add(new TreeNode<String>("Node.2"));
                                    add(new TreeNode<String>("Node.2.1"));
                                    add(new TreeNode<String>("Node.2.1.3"));
                                  }});
        addtlNode.addChildren(new TreeNode<>("Node.2.1.3.1"));
        addtlNode.addChildren(new TreeNode<>("Node.2.1.3.2"));
        addtlNode.addChildren(new TreeNode<>("Node.2.1.3.3"));

        // visualize the tree using plain old ASCII
        TreeAsciiRenderer tv = new TreeAsciiRenderer(System.err);
        tv.renderHierarchical(tree);

        addtlNode.setAsRoot();
        tv.renderHierarchical(addtlNode);

        TreeTraversal<String> tt2 = new BreadthFirstTraversal<>();
        tt2.traversePreOrder(tree, (node) -> {
            System.err.printf("%s (%s - %s)%n", node, node.getLeftSibling(), node.getRightSibling());
            
        });

        // tv.renderFlat(tree);
    }
}
