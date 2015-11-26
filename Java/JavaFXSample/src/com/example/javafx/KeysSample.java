package com.example.javafx;


import java.util.Stack;

import javafx.animation.Interpolator;
import javafx.animation.TranslateTransition;
import javafx.application.Application;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.input.KeyCode;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.stage.Stage;
import javafx.util.Duration;

public class KeysSample extends Application {
    private Rectangle rct_player;
    private TranslateTransition transTransition;

    private Stack<KeyCode> keys = new Stack<>();

    private static double dbl_playerW = 20;
    private static double dbl_playerH = 20;
    private Duration dur = null;
    
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

        Scene scn_main = new Scene(grp_main,400,400);

        // Create the translate transition for the rectangle
        dur = new Duration(1000);       // The time it should take from fromX/Y/Z to toX/Y/Z
        transTransition = new TranslateTransition(dur, rct_player);
        transTransition.setInterpolator(Interpolator.LINEAR);
        //transTransition.setCycleCount(Timeline.INDEFINITE);
        System.err.println("RATE:" + transTransition.getRate());

        scn_main.setOnKeyPressed(e-> {

            KeyCode activeKey = e.getCode();
            if (!keys.contains(activeKey)) {
                handleKey(activeKey);
                keys.push(activeKey);
            }
        });

        scn_main.setOnKeyReleased(e -> {
            keys.remove(e.getCode());
            KeyCode activeKey = KeyCode.UNDEFINED;
            if (!keys.empty()) {
                activeKey = keys.peek();
            }

            handleKey(activeKey);
        });

        stg_main.setScene(scn_main);
        stg_main.setResizable(false);
        stg_main.show();
    }


    private void handleKey(KeyCode activeKey) {
        System.err.println("HANDLE: " + activeKey);
        switch(activeKey) {
            case LEFT  :
//                transTransition.stop();
                transTransition.setToX(0);
                transTransition.setToY(rct_player.getTranslateY());
                transTransition.play();
                break;

            case RIGHT :
//                transTransition.stop();
                transTransition.setToX(400);
                transTransition.setToY(rct_player.getTranslateY());
                transTransition.play();
                break;

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
    
            case SPACE :
                System.err.println("ACTION!");
                //System.err.println("RATE:" + transTransition.getCurrentRate());
                break;

            default :
                System.err.println("STOP!");
                transTransition.stop();
                break;
        }
    }
}
