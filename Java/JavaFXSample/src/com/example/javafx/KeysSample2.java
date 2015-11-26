package com.example.javafx;


import java.util.Stack;

import javafx.animation.Animation;
import javafx.animation.Interpolator;
import javafx.animation.KeyFrame;
import javafx.animation.KeyValue;
import javafx.animation.Timeline;
import javafx.animation.TranslateTransition;
import javafx.application.Application;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.input.KeyCode;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.stage.Stage;
import javafx.util.Duration;

public class KeysSample2 extends Application {
    private Rectangle rct_player;
    //private TranslateTransition transTransition;
    //private Duration dur = null;

    Timeline xlTransition = new Timeline();
    Timeline xrTransition = new Timeline();

    private Stack<KeyCode> keys = new Stack<>();

    private static double dbl_playerW = 20;
    private static double dbl_playerH = 20;
    
    public static void main(String[] args) {
        launch(args);
    }

    public void start(Stage stg_main) {
        Group grp_main = new Group();

        rct_player = new Rectangle();
        rct_player.setFill(Color.BLACK);
        rct_player.setWidth(dbl_playerW);
        rct_player.setHeight(dbl_playerH);
        grp_main.getChildren().add(rct_player);

        rct_player.setTranslateY(100);

        Scene scn_main = new Scene(grp_main,400,400);

        xlTransition.getKeyFrames().add(
                new KeyFrame(Duration.millis(2000),
                new KeyValue (rct_player.translateXProperty(), 400)));
        xlTransition.setRate(-1.0);

        xrTransition.getKeyFrames().add(
                new KeyFrame(Duration.millis(2000),
                new KeyValue (rct_player.translateXProperty(), 400)));
/*        
        // Create the translate transition for the rectangle
        dur = new Duration(1000);       // The time it should take from fromX/Y/Z to toX/Y/Z
        transTransition = new TranslateTransition(dur, rct_player);
        transTransition.setInterpolator(Interpolator.LINEAR);
        //transTransition.setCycleCount(Timeline.INDEFINITE);
        System.err.println("RATE:" + transTransition.getRate());
*/
        scn_main.setOnKeyPressed(e-> {

            KeyCode activeKey = e.getCode();
            if (!keys.contains(activeKey)) {
                handleKey(activeKey, true);
                keys.push(activeKey);
            }
        });

        scn_main.setOnKeyReleased(e -> {
            KeyCode activeKey = e.getCode();
            keys.remove(activeKey);
            handleKey(activeKey, false);

            if (!keys.empty()) {
                activeKey = keys.peek();
                handleKey(activeKey,true);
            }

        });

        stg_main.setScene(scn_main);
        stg_main.setResizable(false);
        stg_main.show();

//        xTransition.setCycleCount(Animation.INDEFINITE);
//        xTransition.setAutoReverse(true);
//        xTransition.play();
//        yTransition.play();
    }


    private void handleKey(KeyCode activeKey, boolean down) {
        System.err.println("\nHANDLE: " + activeKey + "/" + down);

        switch(activeKey) {
            case LEFT  :
              if (down) {
                xrTransition.pause();
                xlTransition.play();
//                    xTransition.play();
//                    //double rate = xTransition.getRate();
//                    //if (rate >= 0) {
//                    xTransition.setRate(-1.0); // -rate);
//                    //}
                } else {
                    xlTransition.pause();
                }
//                transTransition.stop();
//                transTransition.setToX(0);
//                transTransition.setToY(rct_player.getTranslateY());
//                transTransition.play();
                break;

            case RIGHT :

                if (down) {
                    xlTransition.pause();
                    xrTransition.play();
//
//                    xTransition.play();
//                    //double rate = xTransition.getRate();
//                    //System.err.println("   RATE: " + rate);
//                    //if (rate <= 0) {
//                    xTransition.setRate(1.0); // rate);
//                    //}
//                    //System.err.println("  RATE2: " + xTransition.getCurrentRate());
                } else {
                    xrTransition.pause();
                }
//                transTransition.stop();
//                transTransition.setToX(400);
//                transTransition.setToY(rct_player.getTranslateY());
//                transTransition.play();
                break;
/*
            case DOWN  :
                transTransition.stop();
                transTransition.setToX(rct_player.getTranslateX());
                transTransition.setToY(400);
                transTransition.play();
                break;
    
            case UP    :
                transTransition.stop();
                transTransition.setToX(rct_player.getTranslateX());
                transTransition.setToY(0);
                transTransition.play();
                break;
*/

            case SPACE :
//                if (down) {
//                    System.err.println("ACTION!");
//                    System.err.println("RATE:" + xTransition.getCurrentRate());
//
//                    xTransition.setRate(-xTransition.getRate());
//                }

                break;
/*
            default :
                System.err.println("STOP!");
                transTransition.stop();
                break;*/
        }

    }
}
