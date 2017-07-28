javac -d mods/com.example -p ..\Java9Module\com.example.lib.jar src\module-info.java src\com\example\ModuleExample.java
java --module-path mods;..\Java9Module\com.example.lib.jar  -m com.example/com.example.ModuleExample

