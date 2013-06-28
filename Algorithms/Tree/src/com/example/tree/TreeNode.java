package com.example.tree;

import java.util.ArrayList;
import java.util.List;


/**
 * A tree node for an arbitrary n-ary tree.
 *
 * Note that the Java runtime does not provide a tree data structure by default.
 * The TreeSet and TreeMap classes from java.util are an implementation of the
 * Collection interface, using a tree structure to allow sorted collections.
 * But they are not intended as general purpose tree structures.
 *
 * Note also that it is not necessary to have a specific Tree class which holds
 * the root node - each TreeNode can be seen as a Tree itself. This makes using
 * sub trees much easier.
 *
 * javax.swing.tree provides the TreeModel and TreeNode interfaces and specific
 * default implementations to be used with a JTree. They could also be used as 
 * general purpose tree structure (e.g. using the DefaultMutableTreeNode class).
 *
 * See also
 *   http://stackoverflow.com/questions/8641883/why-java-collections-api-does-not-have-tree-implementation
 *   http://stackoverflow.com/questions/3522454/java-tree-data-structure
 *   http://docs.oracle.com/javase/7/docs/api/javax/swing/tree/TreeNode.html
 */
public class TreeNode <T> {
   private List<TreeNode<T>> children = new ArrayList<>();
   private TreeNode<T> parent;
   private T content;

   
   /**
    * Creates a new tree node.
    *
    * @param content The content of the node.
    */
   public TreeNode(T content) {
      this.content = content;
   }

   
   /**
    * 
    * @return The content of this tree node.
    */
   public T getContent() {
      return content;
   }


   @Override
   public String toString() {
      return content.toString();
   }


   /**
    * @return A list of all children of this tree node.
    */
   public List<TreeNode<T>> getChildren() {
      return children;
   }


   /**
    * Adds a child node to this tree node.
    * 
    * @param child The child node to add.
    */
   public void addChildren(TreeNode<T> child) {
      children.add(child);
      child.setParent(this);
   }


   /**
    * @return The path to the root node in a String format.
    */
   public String getPathString() {
      StringBuffer result = new StringBuffer();

      TreeNode<T> node = this;
      do {
         result.insert(0,  "/" + node.getContent().toString());
         node = node.getParent();
      }while(node != null);

      return result.toString();
   }


   /**
    * @return The depth of this tree node within the tree (the distance to the
    *         root node)
    */
   public int getLevel() {
      int result = 0;   // root level

      TreeNode<T> node = getParent();
      while(node != null) {
         result++;
         node = node.getParent();
      }

      return result;
   }


   /**
    * @return The path to the root node as a array of TreeNode objects.
    */
   public TreeNode<?>[] getPath() {
      TreeNode<?>[] result = new TreeNode<?>[getLevel()];

      int i = 0;
      TreeNode<T> node = this;
      do {
         result[i] = node;
         node = node.getParent();
         i++;
      }while(node != null);

      return result;
   }

   
   private void setParent(TreeNode<T> parent) {
      this.parent = parent;  
   }


   /**
    * @return The parent node of this tree node or <code>null</code> if this is
    *         the root node. 
    */
   public TreeNode<T> getParent() {
      return parent;
   }
}
