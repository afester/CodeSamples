package com.example;

import java.io.BufferedOutputStream;
import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintStream;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.List;

public class JdbcTest {

   private Connection con;

   public static void main(String[] args) {
      JdbcTest jdbcTest = new JdbcTest();
      jdbcTest.run();
   }


   public void run() {
      DatabaseProperties props = new DatabaseProperties();

      try {
         Class.forName(props.getDriver());
         con = DriverManager.getConnection(props.getUrl(), props.getUser(), props.getPassword());
         System.out.println("Connected: " + con);
      } catch (ClassNotFoundException e1) {
         e1.printStackTrace();
      } catch (SQLException e) {
         e.printStackTrace();
      }

     // simpleQuery();
      //hierarchicalQuery();
      //for (int i = 0;  i < 3;  i++) {
      //   System.err.printf("%d. Iteration:\n-----------------------------\n", i);
      //   performanceQuery();
     // }
      //monitorInvalids();
      getInvalidViews();

      try {
         con.close();
      } catch (SQLException e) {
         e.printStackTrace();
      }
   }



   public void simpleQuery() {
      try {
         Statement s = con.createStatement();
         ResultSet rs = s.executeQuery("SELECT * FROM City");
         while(rs.next()) {
            System.out.println(rs.getString(1) + " " + rs.getString(2));
         }
         rs.close();
         s.close();
      } catch (SQLException e) {
         e.printStackTrace();
      }

//      s = con.createStatement();
//      s.executeQuery("BEGIN\n"+
//                     "    EXECUTE IMMEDIATE 'DROP TABLE Person';\n"+
//                     "END;");

   }

   
   private void hierarchicalQuery() {
      
      try {
         Statement s = con.createStatement();

         ResultSet rs = s.executeQuery(
               "SELECT LEVEL, Id, Label "+
               "FROM TreeNode "+
               "START WITH Parent = 0 "+
               "CONNECT BY PRIOR Id = Parent");
         while(rs.next()) {
            System.out.println(rs.getString(1) + " " + rs.getString(2) + " " + rs.getString(3));
         }

         rs.close();
         s.close();
      } catch (SQLException e) {
         e.printStackTrace();
      }
   }
   
   private void performanceQuery()  {
      try {
         Statement s = con.createStatement();
         s.setFetchSize(4001);

         List<String> myResult = new ArrayList<>();

         RealTimeCounter rtc = new RealTimeCounter();
         MemoryProbe memp = new MemoryProbe();
         memp.start();
         rtc.start();

         ResultSet rs = s.executeQuery("SELECT * FROM Performance");

         rtc.stop();
         System.err.println(rtc);
         rtc.start();
         while(rs.next()) {
            myResult.add(rs.getString(1));
            //System.out.println(rs.getString(1) + " " + rs.getString(2) + " " + rs.getString(3));
         }
         rtc.stop();
         memp.stop();
         System.err.println(rtc);
         System.err.printf("Consumed memory: %dk\n", memp.getFreeMemoryDelta() / 1024);

         rs.close();
         s.close();
      } catch (SQLException e) {
         e.printStackTrace();
      }
   }

   
    private void monitorInvalids() {
        DateFormat df = new SimpleDateFormat("dd.MM.YY HH:MM:ss");
        try {
            Statement s = con.createStatement();

            while(true) {
                ResultSet rs = s.executeQuery("SELECT COUNT(*) "
                        + "FROM DBA_OBJECTS "
                        + "WHERE OBJECT_TYPE='VIEW' AND STATUS='INVALID'");
                rs.next();
                
                System.out.printf("[%s] Invalid views: %s\n", df.format(System.currentTimeMillis()), rs.getString(1));
                rs.close();
                try {
                    Thread.sleep(2500);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

//            s.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
    
    
    private void getInvalidViews() {

        try {
            PrintStream result = new PrintStream("views.sql");
            Statement s = con.createStatement();

            ResultSet rs = s.executeQuery(
                      "SELECT OBJECT_NAME, TEXT_VC "
                    + "FROM DBA_OBJECTS "
                    + "JOIN DBA_VIEWS ON (OBJECT_NAME=VIEW_NAME) "
                    + "WHERE OBJECT_TYPE='VIEW' AND STATUS='INVALID' "
                    + "ORDER BY OBJECT_NAME");
            while(rs.next()) {
                //result.printf("CREATE OR REPLACE VIEW %s AS %s;\n\n", rs.getString(1), rs.getString(2));
                System.err.printf("%s\n", rs.getString(1));
            }

            rs.close();
            s.close();

            result.close();
        } catch (SQLException e) {
            e.printStackTrace();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }

}
