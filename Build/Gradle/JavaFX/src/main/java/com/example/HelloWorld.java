package com.example;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.layout.HBox;
import javafx.stage.Stage;

public class HelloWorld extends Application {

	public static void main(String[] args) {
		launch(args);
	}

	@Override
	public void start(Stage primaryStage) throws Exception {
		var mainLayout = new HBox();
		mainLayout.getChildren().add(new Button("Hello World"));
		
        Scene scene = new Scene(mainLayout);
        primaryStage.setScene(scene);
        primaryStage.show();

	}
}
