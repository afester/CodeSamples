package com.example.javafx;

import javafx.animation.KeyFrame;
import javafx.animation.KeyValue;
import javafx.animation.Timeline;
import javafx.application.Application;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.layout.HBox;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.stage.Stage;
import javafx.util.Duration;

public class AnimationSample extends Application {
    private Rectangle rect = new Rectangle();
    private Timeline timeline = new Timeline();

    public static void main(String[] args) {
        launch(args);
    }

    public void start(Stage stage) {

        // setup timeline
        timeline.getKeyFrames().add(
                new KeyFrame(Duration.millis(5000),
                new KeyValue (rect.translateXProperty(), 400)));

        // create rectangle to move
        rect.setFill(Color.BLACK);
        rect.setWidth(20);
        rect.setHeight(20);
        rect.setTranslateY(100);

        // create left, pause, right buttons
        HBox buttons = new HBox();
        Button butLeft = new Button("<");
        Button butPause = new Button("||");
        Button butRight = new Button(">");
        buttons.getChildren().addAll(rect, butLeft, butPause, butRight);

        // LEFT action
        butLeft.setOnAction(a -> { 
            //timeline.setRate(-1.0);  // does not work at all - animation starts at end
            timeline.play();
            timeline.setRate(-1.0);    // works as long as the animation was not at the end
        } );

        // PAUSE action
        butPause.setOnAction(a -> timeline.pause());

        // RIGHT action
        butRight.setOnAction(a -> { 
            //timeline.setRate(1.0);  // does not work at all
            timeline.play();
            timeline.setRate(1.0);    // works as long as the animation was not at the beginning
        } );

        // JavaFX boilerplate - setup stage
        Group grp_main = new Group();
        grp_main.getChildren().addAll(buttons, rect);
        Scene scn_main = new Scene(grp_main,400,400);
        stage.setScene(scn_main);
        stage.setResizable(false);
        stage.show();
    }
}
