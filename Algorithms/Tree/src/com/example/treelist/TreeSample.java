package com.example.treelist;

import java.util.ArrayList;
import java.util.List;


public class TreeSample {

   // The tree is represented as a list of tree nodes
   private List<TreeNode> tree;

   public void createSubTree(int level, int childCount, int levelCount, String label) {

      String newLabel;

      for (int cCount = 1;  cCount <= childCount;  cCount++) {
         newLabel = label + '.' + cCount;

         if (cCount < childCount) {
            tree.add(new TreeNode(level, newLabel));
         } else {
            tree.add(new TreeNode(level, newLabel)); // , true));
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
   public List<TreeNode> createTree(int childCount, int depth) {
      tree = new ArrayList<>();

      tree.add(new TreeNode(0, "Root")); // , false));
      createSubTree(1, childCount, depth, "Node");
      
      return tree;
   }


   public void run()  {
      // Create a sample tree
      List<TreeNode> tree = createTree(3, 3);

      int idx = tree.indexOf(new TreeNode(3, "Node.2.3.1"));
      tree.add(idx + 1, new TreeNode(4, "Node.2.3.1.2")); // , true));
      tree.add(idx + 1, new TreeNode(4, "Node.2.3.1.1")); // , false));

      tree.add(new TreeNode(0, "Root.2")); // , true));
      tree.add(new TreeNode(1, "Node.R2.1"));
      tree.add(new TreeNode(1, "Node.R2.2"));

      int iter = tree.size();
      while(iter > 1) {
          iter--;
          if (tree.get(iter).getLevel() < tree.get(iter-1).getLevel()) {
              tree.get(iter-1).setIsLast(true);
          }
      }

      // visualize the tree using plain old ASCII
      TreeAsciiRenderer tv = new TreeAsciiRenderer(System.err);
      tv.renderFlat(tree);
      tv.renderHierarchical(tree);
   }

/*
   public void run2() {
       // Create a sample tree
       List<TreeNode> tree = new ArrayList<>();

tree.add(new TreeNode(0, "X0", true));
       tree.add(new TreeNode(1, "B"));
tree.add(new TreeNode(2, "X1", true));
       tree.add(new TreeNode(3, "D"));
       tree.add(new TreeNode(1, "E"));
tree.add(new TreeNode(2, "X2", true));
       tree.add(new TreeNode(3, "G"));
tree.add(new TreeNode(1, "X3", true));
       tree.add(new TreeNode(2, "I"));

       // visualize the tree using plain old ASCII
       TreeAsciiRenderer tv = new TreeAsciiRenderer(System.err);
       System.err.println("LIST VIEW:");
       tv.renderFlat(tree);
       System.err.println("TREE VIEW:");
       tv.renderHierarchical(tree);

   }
*/
   public static void main(String[] args) {
      new TreeSample().run();
   }
}
