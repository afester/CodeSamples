package com.example.tree;


/**
 * This is a tree where the nodes are made of specific TreeNode objects.
 * Each TreeNode object has a list of its child nodes.
 */
public class Tree {
   private TreeNode rootNode;


   public Tree(String rootLabel) {
      rootNode = new TreeNode(rootLabel);
   }

   public TreeNode getRootNode() {
      return rootNode;
   }

   public void add(TreeNode parent, TreeNode child) {
      parent.addChildren(child);
   }
}
