package com.example.treelist;

import java.io.PrintStream;
import java.util.List;


public class TreeAsciiRenderer {

   private final static char   BAR         = '|';
   private final static char   NO_BAR      = ' ';
   private final static String SPACER      = "  ";
   private final static char   NODE        = '+';
   private final static char   LAST_NODE   = '`';
   private final static String NODE_HANDLE = "--";
   private PrintStream out = null;
   
   public TreeAsciiRenderer(PrintStream ps) {
       out = ps;
   }


   public void renderHierarchical(List<TreeNode> tree) {
      boolean[] hideBar = new boolean[100];  // default is false!

      for (TreeNode tn : tree) {
         StringBuffer line = new StringBuffer();

         // Create the prefix for the current level
         for (int l = 0;  l < tn.getLevel();  l++) {
            if (!hideBar[l]) {
               line.append(BAR);
            } else {
               line.append(NO_BAR);
            }
            line.append(SPACER);
         }

         // reset all bar hiding flags (TODO: HACK!!)
         for (int l = tn.getLevel();  l < 99;  l++) {
            hideBar[l] = false;
         }

         // if this is the last node of the current level, then do not print
         // bars anymore.
         if (tn.isLast()) {
            line.append(LAST_NODE);
            hideBar[tn.getLevel()] = true;
         } else {
            line.append(NODE);
         }
         line.append(NODE_HANDLE + tn.getLabel());

         out.println(line);
      }
   }


   public void renderFlat(List<TreeNode> tree) {
      for (TreeNode tn : tree) {
         out.println(tn);
      }
   }
}
