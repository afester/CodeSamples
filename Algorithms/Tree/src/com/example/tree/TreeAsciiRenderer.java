package com.example.tree;

import java.util.Iterator;


public class TreeAsciiRenderer {

   private final static char   BAR         = '|';
   private final static char   NO_BAR      = ' ';
   private final static String SPACER      = "  ";
   private final static char   NODE        = '+';
   private final static char   LAST_NODE   = '`';
   private final static String NODE_HANDLE = "--";

   
   private void processChildren(TreeNode<?> parent, String prefix) {

      Iterator<?> childs = parent.getChildren().iterator();
      while(childs.hasNext()) {
         TreeNode<?> child = (TreeNode<?>) childs.next();

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
         line.append(child.getContent());

         System.out.println(line);

         processChildren(child, newPrefix.toString());
      }
   }


   /**
    * Renders a tree as a vertical hierarchical structure.
    *
    * @param tree The tree to render.
    */
   public void renderHierarchical(TreeNode<?> tree) {
      // TreeNode rootNode = tree.getRootNode();
      System.out.println(LAST_NODE + NODE_HANDLE + tree.getContent());
      processChildren(tree, "   ");
   }


   private void printNode(TreeNode<?> node) {
      System.out.println(node.getLevel() + " " + node + " (" + node.getPathString() + ")");
      for (TreeNode<?> child : node.getChildren()) {
         printNode(child);
      }
   }


   /**
    * Renders a tree as a flat list.
    * 
    * @param tree The tree to render.
    */
   public void renderFlat(TreeNode<?> tree) {
      printNode(tree);
   }
}
