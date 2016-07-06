package com.example.tree;

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

    private void run() {
        // Create a sample tree
        TreeNode<String> tree = createTree(3, 3);

        // visualize the tree using plain old ASCII
        TreeAsciiRenderer tv = new TreeAsciiRenderer(System.err);
        tv.renderHierarchical(tree);

        // tv.renderFlat(tree);
    }
}
