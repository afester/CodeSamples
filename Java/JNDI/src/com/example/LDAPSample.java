package com.example;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
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
import javax.naming.directory.DirContext;
import javax.naming.directory.InitialDirContext;


public class LDAPSample {
   private DirContext ctx; 

   public void run() {
      // create the configuration environment
      Hashtable<String, Object> env = createLdapEnv();

      // Create the initial context
      try {
         ctx = new InitialDirContext(env);

         listNode("");
         //listNode("dc=DefaultDomain");
         //listNode("ou=myrealm,dc=DefaultDomain");
         //listNode("ou=Policies,ou=XACMLRole,ou=myrealm,dc=DefaultDomain");
         //listNode("cn=schema");
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

   
   private void listAttributes(String cn) throws NamingException {
      System.err.println("Attributes in \"" + cn + "\"\n====================================");

      Attributes attrs = ctx.getAttributes(cn);
      NamingEnumeration<? extends Attribute> attributes = attrs.getAll();
      while(attributes.hasMore()) {
         Attribute attr = attributes.next();

         List<String> attrValues = new ArrayList<>();
         NamingEnumeration<?> attributes2 = attr.getAll();
         while(attributes2.hasMore()) {
            attrValues.add(attributes2.next().toString());
         }

         if (attrValues.size() > 1) {
            System.err.println(attr.getID() + " = " + attrValues);
         } else {
            System.err.println(attr.getID() + " = " + attrValues.get(0));
         }
      }
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
