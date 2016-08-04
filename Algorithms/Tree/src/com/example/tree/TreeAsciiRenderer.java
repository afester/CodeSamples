package com.example.tree;

import java.io.PrintStream;
import java.util.Iterator;


public class TreeAsciiRenderer<T> {

   private final static char   BAR         = '|';
   private final static char   NO_BAR      = ' ';
   private final static String SPACER      = "  ";
   private final static char   NODE        = '+';
   private final static char   LAST_NODE   = '`';
   private final static String NODE_HANDLE = "--";
   private PrintStream out;

   public TreeAsciiRenderer(PrintStream ps) {
       out = ps;
   }


   private void processChildren(TreeNode<T> parent, String prefix) {

      Iterator<TreeNode<T>> childs = parent.getChildren().iterator();
      while(childs.hasNext()) {
         TreeNode<T> child = childs.next();

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

         out.println(line);

         processChildren(child, newPrefix.toString());
      }
   }


   /**
    * Renders a tree as a vertical hierarchical structure.
    *
    * @param tree The tree to render.
    */
   public void renderHierarchical(TreeNode<T> tree) {
      out.println(tree.getContent());
      processChildren(tree, "");
   }



   
   private void printNode(TreeNode<T> node) {
      out.println(node.getLevel() + " " + node + " (" + node.getPathString() + ")");
      for (TreeNode<T> child : node.getChildren()) {
         printNode(child);
      }
   }


   /**
    * Renders a tree as a flat list.
    * 
    * @param tree The tree to render.
    */
   public void renderFlat(TreeNode<T> tree) {
      printNode(tree);
   }



    private String getPrefix(int level) {
        return new String(new char[level]).replace("\0", "+");
    }

    /**
     * Renders a tree as a flat list.
     * 
     * @param tree The tree to render.
     */
    public void renderExt(TreeNode<T> tree) {
       DepthFirstTraversal<T> trav = new DepthFirstTraversal<>();
       trav.traversePreOrder(tree, e -> out.printf("%s%s%n", getPrefix(e.getLevel()), e) ); 
    }
}

