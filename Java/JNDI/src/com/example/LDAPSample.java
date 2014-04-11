package com.example;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.List;
import java.util.Properties;

import javax.naming.Binding;
import javax.naming.Context;
import javax.naming.NamingEnumeration;
import javax.naming.NamingException;
import javax.naming.directory.Attribute;
import javax.naming.directory.Attributes;
import javax.naming.directory.BasicAttributes;
import javax.naming.directory.DirContext;
import javax.naming.directory.InitialDirContext;
import javax.naming.directory.SearchResult;



public class LDAPSample {
   private DirContext ctx; 
 

   
   
   public void run() {
      // create the configuration environment
      Hashtable<String, Object> env = createLdapEnv();

      // Create the initial context
      try {
         ctx = new InitialDirContext(env);

         //listNode("");
         //listNode("dc=DefaultDomain");
         //listNode("ou=myrealm,dc=DefaultDomain");
         //listNode("ou=Policies,ou=XACMLRole,ou=myrealm,dc=DefaultDomain");
         //listNode("cn=schema");
         //listNode("uid=john.doe@mail.com,ou=people,ou=myrealm,dc=DefaultDomain");

         Attributes attr = new BasicAttributes();
         attr.put("uid",  "andreas.fester@oracle.com");
         NamingEnumeration<SearchResult> result = ctx.search("ou=people,ou=myrealm,dc=DefaultDomain",  attr);
         int count = 0;
         while(result.hasMore()) {
            count++;

            SearchResult entry = result.next();
            System.err.println(entry.getName());
            listAttributes(entry.getAttributes().getAll());
         }
         System.err.println("Found: " + count + " entries.");

      } catch (NamingException e) {
         e.printStackTrace();
      }
   }


   private Hashtable<String, Object> createLdapEnv() {
      Properties p = new Properties();
      try {
         p.load(new FileInputStream("ldap.properties"));
      } catch (FileNotFoundException e1) {
         e1.printStackTrace();
      } catch (IOException e1) {
         e1.printStackTrace();
      }

      String ldapUrl = p.getProperty("ldap.url", "ldap://localhost");
      String ldapAuthScheme = p.getProperty("ldap.authScheme", "simple");
      String ldapUser = p.getProperty("ldap.user", "cn=User");
      String ldapPassword = p.getProperty("ldap.password", "");
      String ldapTracing = p.getProperty("ldap.tracing", "false");

      Hashtable<String, Object> env = new Hashtable<String, Object>();
      env.put(Context.INITIAL_CONTEXT_FACTORY, "com.sun.jndi.ldap.LdapCtxFactory");
      env.put(Context.PROVIDER_URL, ldapUrl);
      env.put(Context.SECURITY_AUTHENTICATION, ldapAuthScheme); 
      env.put(Context.SECURITY_PRINCIPAL, ldapUser);
      env.put(Context.SECURITY_CREDENTIALS, ldapPassword);
      if (ldapTracing.equals("true")) {
         env.put("com.sun.jndi.ldap.trace.ber", System.err);
      }

      return env;
   }


   private void listNode(String cn) {
      try {
         listAttributes(cn);
         System.err.println();
         listBindings(cn);
      }catch(NamingException ne) {
         ne.printStackTrace();
      }
   }

   
   private void listAttributes(NamingEnumeration<? extends Attribute> attributes) throws NamingException {
      while(attributes.hasMore()) {
         Attribute attr = attributes.next();

         List<Object> attrValues = new ArrayList<>();
         NamingEnumeration<?> values = attr.getAll();
         while(values.hasMore()) {
            attrValues.add(values.next());
         }

         if (attrValues.size() > 1) {
            System.err.println(attr.getID() + " = " + attrValues);
         } else {
            Object value = attrValues.get(0);
            if (value instanceof byte[]) {
               System.err.println(attr.getID() + " = ");

               byte[] data = (byte[]) value;
               HexDump dump = new HexDump();
               dump.setPrefix("    ");
               dump.dump(data);
               dump.done();

               String str = new String(data, StandardCharsets.US_ASCII);
               if (str.toLowerCase().startsWith("{ssha}")) {
                  String base64enc = str.substring(6);
                  // System.err.println(base64enc);
                  System.err.println("  BASE64 decoded password:");
                  byte[] decoded = javax.xml.bind.DatatypeConverter.parseBase64Binary(base64enc);
                  dump = new HexDump();
                  dump.setPrefix("    ");
                  dump.dump(decoded);
                  dump.done();
               }
            } else {
               System.err.println(attr.getID() + " = " + value);
            }
         }
      }
   }

   
   private void listAttributes(String cn) throws NamingException {
      System.err.println("Attributes in \"" + cn + "\"\n====================================");

      Attributes attrs = ctx.getAttributes(cn);
      listAttributes(attrs.getAll());
   }


   private void listBindings(String cn) throws NamingException {
      System.err.println("Bindings in \"" + cn + "\"\n====================================");

      NamingEnumeration<Binding> bindings = ctx.listBindings(cn);
      while(bindings.hasMore()) {
         Binding b = bindings.next();
         System.err.println(b.getName());
      }
   }


   public static void main(String[] args) {
      new LDAPSample().run();
   }
}
