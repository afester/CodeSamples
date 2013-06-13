Deployment on Tomcat
====================

NOTE: The WEB-INF sub directory is not part of the document root - therefore its
contents are not served! 

This sample applet supports various deployment scenarios:

* Local - just open the index.html or one of the from*.html files through a 
  file:// URL in the browser. The applet might have the permission to run or not,
  depending on the security settings.
  
* Served through an application server

  Deploy the application on a J2EE application server (test platform was 
  Tomcat 7) and open http://localhost:8080/Applet. This shows the index.html
  where different launch mechanism can be selected:

  Run Applet from the class files - directly loads the applet's class files from
                                    the "classes" directory.
  
  Run Applet from a jar file - loads the unsigned applet jar file from the "jars"
                               directory. 

  Run Applet from a signed jar file inside the sandbox - loads the signed applet
                               jar file from the "jars" directory, but still runs
                               the applet with restricted permissions in the sandbox
  
  Run Applet from a signed jar file outside the sandbox - loads the signed applet
                               jar file from the "jars" directory, and grants
                               all permissions to the applet (applet runs outside
                               of the sandbox)

Self-signing the jar file
=========================

    Creating the key pair
    =====================
    
    $ keytool -genkey -alias signFiles -keystore mykeys.dat
    Enter keystore password: mykeys
    Re-enter new password: mykeys
    What is your first and last name?
      [Unknown]:  Example
    What is the name of your organizational unit?
      [Unknown]:  example
    What is the name of your organization?
      [Unknown]:  Example inc.
    What is the name of your City or Locality?
      [Unknown]:  Sampletown
    What is the name of your State or Province?
      [Unknown]:  Sample
    What is the two-letter country code for this unit?
      [Unknown]:  ex
    Is CN=Example, OU=example, O=Example inc., L=Sampletown, ST=Sample, C=ex correct?
      [no]:  yes
    
    Enter key password for <signFiles>
            (RETURN if same as keystore password): codesigning
    Re-enter new password: codesigning
    
    
    Signing the jar file (done through the ant script)
    ==================================================
    
    $ jarsigner -keystore mykeys.dat -signedjar sampleApplet_s.jar sampleApplet.jar signFiles 


Caching
=======

In order to reload the changed sources each time they have been modified,
the following things have been suggested:

- Disable caching in the http-header:
      <meta http-equiv="cache-control" content="no-cache" />
      
- Disable caching of the applet:
      <object ...>
          <param name="cache_option" value="No" />
      </object>
  See also http://docs.oracle.com/javase/6/docs/technotes/guides/deployment/deployment-guide/applet-caching.html
  
HOWEVER, NONE OF THESE WORKED - in particular, disabling http caching will not
affect the applet plugin anyways since it refers to the html content only.
Not sure if the "cache_option" parameter is also available with the <applet> tag-
at least, it did not work.

THE ONLY THING WHICH WORKED (at least on Chrome) WAS: clear the classloader 
cache through the java applet plugin console by pressing "x".


HTML Tags
=========

http://download.java.net/jdk8/docs/technotes/guides/jweb/applet/using_tags.html

You use the applet tag to deploy applets to a multi-browser environment.

Note: The HTML specification states that the applet tag is deprecated, and that 
you should use the object tag instead. However, the specification is vague about
how browsers should implement the object tag to support Java applets, and 
browser support is currently inconsistent. Oracle therefore recommends that you
continue to use the applet tag as a consistent way to deploy Java applets across
browsers on all platforms.


<!-- IE specific: -->

<OBJECT 
  classid="clsid:8AD9C840-044E-11D1-B3E9-00805F499D93"
  width="200" height="200">
  <PARAM name="code" value="Applet1.class">
</OBJECT>
