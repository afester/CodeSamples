package com.example.treelist;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

/**
 * This is a tree where the nodes are stored in a flat list, together
 * with their level.
 */
public class Tree implements Iterable<TreeNode> {
   private List<TreeNode> tree = new ArrayList<>();

   public boolean add(TreeNode e) {
      return tree.add(e);
   }

   public void add(int index, TreeNode element) {
      tree.add(index, element);
   }

   public int indexOf(Object o) {
      return tree.indexOf(o);
   }

   @Override
   public Iterator<TreeNode> iterator() {
      return tree.iterator();
   }
}
