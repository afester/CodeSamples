package com.example.javafx;

import javafx.animation.Animation;
import javafx.animation.KeyFrame;
import javafx.animation.KeyValue;
import javafx.animation.Timeline;
import javafx.animation.TimelineBuilder;
import javafx.application.Application;
import javafx.beans.binding.Bindings;
import javafx.beans.property.IntegerProperty;
import javafx.beans.property.SimpleIntegerProperty;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.HBox;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.stage.Stage;
import javafx.util.Duration;

public class GameLoopSample extends Application {
    private IntegerProperty ip = new SimpleIntegerProperty();

    public static void main(String[] args) {
        launch(args);
    }

    public void start(Stage stage) {

        // setup timeline  duration is the complete cycle (100000 / 400 = 250 msec!)
        final Duration oneFrameAmt = Duration.millis(2000); // 1000 / 60);
        final KeyValue target =  new KeyValue (ip, 10);
        final KeyFrame oneFrame = new KeyFrame(oneFrameAmt, target);
/*                new EventHandler() {

                @Override
                public void handle(javafx.event.ActionEvent event) {
                }
             },
             target);
*/
        HBox uiGroup = new HBox();
        final Label lbl = new Label();
        uiGroup.getChildren().addAll(lbl);
        lbl.textProperty().bind(Bindings.convert(ip));

        Timeline timeline = new Timeline();
        timeline.getKeyFrames().add(oneFrame);
        timeline.setCycleCount(Animation.INDEFINITE);
        timeline.play();

        // JavaFX boilerplate - setup stage
        Group grp_main = new Group();
        grp_main.getChildren().addAll(uiGroup);
        Scene scn_main = new Scene(grp_main,400,400);
        stage.setScene(scn_main);
        stage.setResizable(false);
        stage.show();
    }
}
