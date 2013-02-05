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
/*
   private Badge(
         int badge_id,
         String rank,
         String name,
         String award_count,
         String badge_type,
         String link) {

     this.badge_id = badge_id;
     this.rank = rank;
     this.name = name;
     this.award_count = award_count;
     this.badge_type = badge_type;
     this.link = link;
   }
*/

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
