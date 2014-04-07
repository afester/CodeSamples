- Create the libs.jar file:

jar cvfm libs.jar manifest.txt

- Compile the sample:

javac -d bin -classpath libs.jar -sourcepath src src/com/example/Sample.java
