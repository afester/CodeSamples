package com.example.tree;


public class TreeSample {

   private TreeNode<String> tree;


   public void createSubTree(TreeNode<String> parent, int childCount, int levelCount, String label) {

      String newLabel;

      for (int cCount = 1;  cCount <= childCount;  cCount++) {
         newLabel = label + '.' + cCount;
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
   public TreeNode<String> createTree(int childCount, int depth) {
      tree = new TreeNode<>("Root");
      createSubTree(tree, childCount, depth, "Node");

      return tree;
   }


   public void run()  {
      // Create a sample tree
      TreeNode<String> tree = createTree(3, 3);
      
      TreeNode[] path = {new TreeNode<String>("Node.2"), 
                         new TreeNode<String>("Node.2.3"),
                         new TreeNode<String>("Node.2.3.1")};
      TreeNode<String> node = tree.findNode(path);
      System.err.println(node.getPathString());
      for (int i = 0;  i < 500;  i++) {
         node.addChildren(new TreeNode<String>("Node.2.3.1." + i));
      }

      // visualize the tree using plain old ASCII
      TreeAsciiRenderer tv = new TreeAsciiRenderer();
      tv.renderHierarchical(tree);
      tv.renderFlat(tree);
   }


   public static void main(String[] args) {
      new TreeSample().run();
   }
}
