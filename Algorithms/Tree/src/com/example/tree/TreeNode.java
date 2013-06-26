package com.example.tree;

import java.util.ArrayList;
import java.util.List;

public class TreeNode {
   private List<TreeNode> children = new ArrayList<>();
   private String label;

   public TreeNode(String label) {
      this.label = label;
   }

   public String getLabel() {
      return label;
   }

   public String toString() {
      return label;
   }

   
   public List<TreeNode> getChildren() {
      return children;
   }


   void addChildren(TreeNode child) {
      children.add(child);
   }
}

