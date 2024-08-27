package de.labcorner.javafxminimal;

import javafx.application.Application;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.stage.Stage;

public class Main extends Application {

    public static void main(String[] args) {
    	launch(args);
    }

	@Override
	public void start(Stage primaryStage) throws Exception {
        var r1 = new Rectangle(100, 100);
        r1.setFill(Color.RED);

        var group = new Group();
        group.getChildren().add(r1);
		var scene = new Scene(group, 800, 600);
		
        primaryStage.setScene(scene);
        primaryStage.show();
	}
}
