package com.example


import javafx.application.Application
import javafx.stage.Stage
import javafx.scene.Scene
import javafx.scene.control.Button
import javafx.scene.layout.HBox

class JavaFXSample extends Application {

	@Override
	public void start(Stage primaryStage) throws Exception {

		def mainGroup = new HBox() 

		mainGroup.getChildren().add(new Button("First"))
		mainGroup.getChildren().add(new Button("Second"))
		mainGroup.getChildren().add(new Button("Third"))

		Scene scene = new Scene(mainGroup, 800, 600);
		
		primaryStage.setScene(scene);
		primaryStage.show();
	}
}
