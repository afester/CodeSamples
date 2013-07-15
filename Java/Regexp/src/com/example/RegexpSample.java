package com.example;

import java.util.Arrays;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class RegexpSample {

   // http://www.night-ray.com/regex.pdf
   public static void main(String[] args) {
      new RegexpSample().run();
   }
   

   public void run() {

      String str = "Hello World @12:00";

      System.out.println(str.matches(".*@12.*"));
      System.out.println(str.matches(".*@13.*"));
      
      // identical to:
      System.out.println(Pattern.matches(".*@12.*", str));
      System.out.println(Pattern.matches(".*@13.*", str));

      // more operations are available through the "Matcher" class:
      Pattern pattern = Pattern.compile(".*@12.*");
      Matcher matcher = pattern.matcher(str);
      System.out.println(matcher.find());
      System.out.println(matcher.group());

      // more operations are available through the "Matcher" class:
      Pattern pattern2 = Pattern.compile("@12");
      Matcher matcher2 = pattern2.matcher(str);
      System.out.println(matcher2.find());
      System.out.println(matcher2.group());

      String str2 = "<Apr 11, 2013 1:45:43 PM BST> <Notice> <WebLogicServer> <BEA-000365> <Server state changed to RESUMING>";
      printMatch("Notice", str2);
      printMatch("<.{3} \\d{1,2}, \\d{4} \\d{1,2}:\\d{2}:\\d{2} PM BST>", str2);
      printMatch("Db(User|Password)", "sampleDbUser");
      printMatch("Db(User|Password)", "sampleDbPassword");
      printMatch("=(.*)", "name=value", 1);
   }


   public void printMatch(String regexp, String str, int group) {
      System.out.println("\nMatching \"" + regexp + "\"\n against \"" + str + "\"");
      Pattern pattern = Pattern.compile(regexp);
      Matcher matcher = pattern.matcher(str);
      System.out.println( String.format("   find(): %b", matcher.find() ));
      System.out.println( String.format("   Result: %d/%d \"%s\"" , matcher.start(), matcher.end(), matcher.group(group)) );
   }
   
   public void printMatch(String regexp, String str) {
      printMatch(regexp, str, 0);
   }
   
}
