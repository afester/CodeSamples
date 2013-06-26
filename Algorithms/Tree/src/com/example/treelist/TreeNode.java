package com.example.treelist;

public class TreeNode {
   @Override
   public int hashCode() {
      final int prime = 31;
      int result = 1;
      result = prime * result + ((label == null) ? 0 : label.hashCode());
      result = prime * result + level;
      return result;
   }

   @Override
   public boolean equals(Object obj) {
      if (this == obj)
         return true;
      if (obj == null)
         return false;
      if (getClass() != obj.getClass())
         return false;
      TreeNode other = (TreeNode) obj;
      if (label == null) {
         if (other.label != null)
            return false;
      } else if (!label.equals(other.label))
         return false;
      if (level != other.level)
         return false;
      return true;
   }

   private int level;
   private String label;
   private boolean isLast;

   public TreeNode(int level, String label) {
      this(level, label, false);
   }

   public TreeNode(int level, String label, boolean isLast) {
      this.level = level;
      this.label = label;
      this.isLast = isLast;
      
   }

   public int getLevel() {
      return level;
   }

   public String getLabel() {
      return label;
   }

   public boolean isLast() {
      return isLast;
   }

   public String toString() {
      return level + " " + label;
   }
}
