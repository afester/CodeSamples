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
