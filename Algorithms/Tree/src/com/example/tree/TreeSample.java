package com.example.tree;


public class TreeSample {

   private Tree tree;

   public void createSubTree(TreeNode parent, int childCount, int levelCount, String label) {

      String newLabel;

      for (int cCount = 1;  cCount <= childCount;  cCount++) {
         newLabel = label + '.' + cCount;
         TreeNode child = new TreeNode(newLabel);
         tree.add(parent, child);
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
    */
   public Tree createTree(int childCount, int depth) {
      tree = new Tree("Root");
      TreeNode rootNode = tree.getRootNode();
      createSubTree(rootNode, childCount, depth, "Node");

      return tree;
   }


   public void run()  {
      // Create a sample tree
      Tree tree = createTree(3, 3);

//      int idx = tree.indexOf(new TreeNode(3, "Node.2.3.1"));
//      tree.add(idx + 1, new TreeNode(4, "Node.2.3.1.2", true));
//      tree.add(idx + 1, new TreeNode(4, "Node.2.3.1.1", false));

      // visualize the tree using plain old ASCII
      TreeVisualizer tv = new TreeVisualizer();
//      tv.visualizeFlat(tree);
      tv.visualizeHierarchical(tree);
   }


   public static void main(String[] args) {
      new TreeSample().run();
   }
}
