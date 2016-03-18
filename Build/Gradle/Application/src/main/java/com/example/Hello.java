package com.example;

import javafx.scene.layout.HBox;
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.stage.Stage;
import javafx.scene.Group;
import javafx.scene.text.Text;
import javafx.scene.control.TextArea;

public class Hello extends Application {
   
   public static void main(String[] args) {
      HelloWriter hw = new HelloWriter();
      hw.sayHello("World");
      launch(args); 
   }

   @Override
   public void start(Stage primaryStage) {
      HBox mainGroup = new HBox();
      TextArea infoArea = new TextArea();
      infoArea.setEditable(false);
      infoArea.appendText(String.format("java.version: %s\n", System.getProperty("java.version")));
      infoArea.appendText(String.format("java.runtime.version: %s\n", System.getProperty("java.runtime.version")));
      mainGroup.getChildren().addAll(infoArea);
      Scene scene = new Scene(mainGroup);
      primaryStage.setScene(scene);
      primaryStage.show();
   }
}

