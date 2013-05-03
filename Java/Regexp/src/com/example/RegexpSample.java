package com.example;

import java.util.Arrays;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class RegexpSample {

   // http://www.night-ray.com/regex.pdf
   public static void main(String[] args) {

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

      Pattern pattern3 = Pattern.compile("Notice");
      Matcher matcher3 = pattern3.matcher(str2);
      System.out.println(matcher3.find());
      System.out.println(matcher3.start() + "/" + matcher3.end() + ":" +  matcher3.group());
      
      pattern3 = Pattern.compile("<.{3} \\d{1,2}, \\d{4} \\d{1,2}:\\d{2}:\\d{2} PM BST>");
      matcher3 = pattern3.matcher(str2);
      System.out.println(matcher3.find());
      System.out.println(matcher3.start() + "/" + matcher3.end() + ":" +  matcher3.group());
   }
}
