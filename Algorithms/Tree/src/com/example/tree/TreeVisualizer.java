package com.example.tree;

import java.util.Iterator;


public class TreeVisualizer {

   private final static char   BAR         = '|';
   private final static char   NO_BAR      = ' ';
   private final static String SPACER      = "  ";
   private final static char   NODE        = '+';
   private final static char   LAST_NODE   = '`';
   private final static String NODE_HANDLE = "--";

   
   private void processChildren(TreeNode parent, String prefix) {

      Iterator<TreeNode> childs = parent.getChildren().iterator();
      while(childs.hasNext()) {
         TreeNode child = childs.next();

         StringBuilder line = new StringBuilder(prefix);
         StringBuilder newPrefix = new StringBuilder(prefix);

         // last child?
         if (childs.hasNext()) {
            line.append(NODE);
            newPrefix.append(BAR);
         } else {
            line.append(LAST_NODE);
            newPrefix.append(NO_BAR);
         }
         newPrefix.append(SPACER);

         line.append(NODE_HANDLE);
         line.append(child.getLabel());

         System.out.println(line);

         processChildren(child, newPrefix.toString());
      }
   }


   public void visualizeHierarchical(Tree tree) {
      TreeNode rootNode = tree.getRootNode();
      System.out.println(LAST_NODE + NODE_HANDLE + rootNode.getLabel());
      processChildren(rootNode, "   ");
   }


   private void printNode(TreeNode node) {
      System.out.println(node);
      for (TreeNode child : node.getChildren()) {
         printNode(child);
      }
   }

   public void visualizeFlat(Tree tree) {
      printNode(tree.getRootNode());
   }
}
