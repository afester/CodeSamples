package net.littletux;

import java.util.ArrayList;
import java.util.Collection;

public class Badges {
   private Collection<Badge> items = new ArrayList<>();
   private int quota_remaining;
   private int quota_max;
   private boolean has_more;

   public String toString() {
      return items.toString() + 
          "\nquota_remaining:" + quota_remaining +
          "\nquota_max:" + quota_max +
          "\nhas_more:" + has_more;
   }
}
