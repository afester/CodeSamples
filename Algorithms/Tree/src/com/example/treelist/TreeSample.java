package com.example.treelist;


public class TreeSample {

   private Tree tree;

   public void createSubTree(int level, int childCount, int levelCount, String label) {

      String newLabel;

      for (int cCount = 1;  cCount <= childCount;  cCount++) {
         newLabel = label + '.' + cCount;

         if (cCount < childCount) {
            tree.add(new TreeNode(level, newLabel));
         } else {
            tree.add(new TreeNode(level, newLabel, true));
         }
         if (levelCount > 1) {
            createSubTree(level + 1, childCount, levelCount - 1, newLabel);
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
      tree = new Tree();

      tree.add(new TreeNode(0, "Root", true));
      createSubTree(1, childCount, depth, "Node");
      
      return tree;
   }


   public void run()  {
      // Create a sample tree
      Tree tree = createTree(3, 3);

      int idx = tree.indexOf(new TreeNode(3, "Node.2.3.1"));
      tree.add(idx + 1, new TreeNode(4, "Node.2.3.1.2", true));
      tree.add(idx + 1, new TreeNode(4, "Node.2.3.1.1", false));

      // visualize the tree using plain old ASCII
      TreeVisualizer tv = new TreeVisualizer();
      tv.visualizeFlat(tree);
      tv.visualizeHierarchical(tree);
   }


   public static void main(String[] args) {
      new TreeSample().run();
   }
}
