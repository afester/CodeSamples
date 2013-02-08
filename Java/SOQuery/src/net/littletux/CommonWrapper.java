package net.littletux;

import java.util.Collection;

public class CommonWrapper<T> {
	 private int backoff; 
	 private int error_id;
	 private String error_message;
	 private String error_name; 
	 private boolean has_more;
	 private Collection<T> items;
	 private int page;
	 private int page_size;
	 private int quota_max;
	 private int quota_remaining;
	 private int total;
	 private String type;

	 public Collection<T> getItems() {
		 return items;
	 }

	 public boolean hasMore() {
		 return has_more;
	 }

	 public String toString() {
		 return items.toString() + 
				 "backoff=" + backoff + 
				 ",error_id=" + error_id +
				 ",error_message=" + error_message +
				 ",error_name=" + error_name+
				 ",has_more=" + has_more +
				 ",page=" + page +
				 ",page_size=" + page_size +
				 ",quota_max=" + quota_max +
				 ",quota_remaining=" + quota_remaining +
				 ",total=" + total +
				 ",type=" + type;
	 }
}
