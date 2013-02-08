package net.littletux;


/**
 * A single entry from the /users response.
 */
public class User {
	private int user_id;
	private String user_type;
	private int creation_date;
	private String display_name;
	private String profile_image;
	private int reputation;
	private int reputation_change_day;
	private int reputation_change_week;
	private int reputation_change_month;
	private int reputation_change_quarter;
	private int reputation_change_year;
	private int age;
	private int last_access_date;
	private int last_modified_date;
	private boolean is_employee;
	private String link;
	private String website_url;
	private String location;
	private int account_id;
/*    
    "badge_counts": {
      "gold": 0,
      "silver": 3,
      "bronze": 19
    }
  }
*/
   public String toString() {
	  String result = "user_id=" + user_id +
			  		  ",user_type=" + user_type +
			  		  ",creation_date=" + creation_date +
			  		  ",display_name=" + display_name;
/*		private String profile_image;
		private int reputation;
		private int reputation_change_day;
		private int reputation_change_week;
		private int reputation_change_month;
		private int reputation_change_quarter;
		private int reputation_change_year;
		private int age;
		private int last_access_date;
		private int last_modified_date;
		private boolean is_employee;
		private String link;
		private String website_url;
		private String location;
		private int account_id;
*/
      return result;
   }
}
