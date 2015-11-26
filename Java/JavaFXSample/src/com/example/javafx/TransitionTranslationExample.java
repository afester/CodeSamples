package com.example.javafx;

import javafx.animation.Interpolator;
import javafx.animation.TranslateTransition;
import javafx.application.Application;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.stage.Stage;
import javafx.util.Duration;

public class TransitionTranslationExample extends Application {


    public static void main(String[] args) {
        launch(args);
    }
    
    public void start(Stage stage) {

        Rectangle node = new Rectangle();
        node.setStroke(Color.BLUE);
        node.setFill(Color.LIGHTGREY);
        node.setWidth(15);
        node.setHeight(30);

        Duration dur = new Duration(2000);       // The time it should take from fromX/Y/Z to toX/Y/Z
        TranslateTransition transTransition = new TranslateTransition(); // dur, node);
        transTransition.setRate(50.0);
        transTransition.setToX(395);
        transTransition.setToY(380);
        transTransition.setInterpolator(Interpolator.LINEAR);

        Group mainGroup = new Group();
        mainGroup.getChildren().add(node);
        Scene scene = new Scene(mainGroup, 400, 400);
        stage.setScene(scene);
        stage.setResizable(false);
        stage.show();
        transTransition.play();
    }
}
