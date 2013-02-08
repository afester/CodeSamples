package net.littletux;


/**
 * A single entry from the /badges response.
 */
public class Badge {
   private int badge_id;
   private String rank;
   private String name;
   private String award_count;
   private String badge_type;
   private String link;

   public String toString() {
      String result = "\nbadge_id: " + badge_id +
                     "\nrank:" + rank + 
                     "\nname: " + name +
                     "\naward_count:" + award_count +
                     "\nbadge_type:" + badge_type +
                     "\nlink:" + link;
      result = result + "\n------------";
      return result;
   }
}
