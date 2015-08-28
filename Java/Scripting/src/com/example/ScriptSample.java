package com.example;

import java.util.List;

import javax.script.ScriptEngine;
import javax.script.ScriptEngineFactory;
import javax.script.ScriptEngineManager;
import javax.script.ScriptException;

public class ScriptSample {
   public static void main(String[] args) {
      ScriptEngineManager manager = new ScriptEngineManager();
      
      // list all available scripting engines
      List<ScriptEngineFactory> factories = manager.getEngineFactories();
      for (ScriptEngineFactory sef : factories) {
         System.err.println(sef.getEngineName() + 
                             "\n  Engine version  : " + sef.getEngineVersion() +
                             "\n  Language        : " + sef.getLanguageName() +
                             "\n  Language version: " + sef.getLanguageVersion() +
                             "\n  Mime types      : " + sef.getMimeTypes());
      }

      ScriptEngine engine = manager.getEngineByMimeType("application/javascript");
      try {
         engine.eval("alert('Hello');");
      } catch (ScriptException e) {
         // TODO Auto-generated catch block
         e.printStackTrace();
      }
   }
}
