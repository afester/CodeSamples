package com.example.javafx;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

import javafx.animation.Animation;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.application.Application;
import javafx.geometry.Bounds;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.input.KeyCode;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Rectangle;
import javafx.stage.Stage;
import javafx.util.Duration;



public class GameLoopSample extends Application {

    enum DIRECTION {NONE, LEFT,RIGHT, UP, DOWN};

    private Rectangle rect = new Rectangle(60, 8);
    private Circle ball = new Circle(10);
    private DIRECTION currentDirection = DIRECTION.NONE;
    private final static double SPEED = 12.0;

    // ball vector
    private double bAngle = Math.toRadians(315);
    private double bSpeed = 4.0;

    public static void main(String[] args) {
        launch(args);
    }

    public static void main2(String[] args) {
        GameLoopSample gls = new GameLoopSample();

        // horizontal barrier, ball from top
        double oldAngle = 7 * Math.PI / 4;  // 315°
        double newAngle = gls.calculateExitAngle(oldAngle, 0);  // 45
        System.err.printf("%s (%s) => %s (%s)\n", oldAngle, Math.toDegrees(oldAngle), newAngle, Math.toDegrees(newAngle));

        oldAngle = Math.PI / 4;  // 45°
        newAngle = gls.calculateExitAngle(oldAngle, 0); // 315
        System.err.printf("%s (%s) => %s (%s)\n", oldAngle, Math.toDegrees(oldAngle), newAngle, Math.toDegrees(newAngle));

        oldAngle = Math.toRadians(80);
        newAngle = gls.calculateExitAngle(oldAngle, 0); // 280
        System.err.printf("%s (%s) => %s (%s)\n", oldAngle, Math.toDegrees(oldAngle), newAngle, Math.toDegrees(newAngle));

        // horizontal barrier, ball from bottom
        oldAngle = Math.toRadians(45);
        newAngle = gls.calculateExitAngle(oldAngle, Math.PI/2); // 135°
        System.err.printf("%s (%s) => %s (%s)\n", oldAngle, Math.toDegrees(oldAngle), newAngle, Math.toDegrees(newAngle));
    }

    public void start(Stage stage) {

        // create rectangle to move
        rect.setFill(Color.BLACK);
        rect.setTranslateY(200);
        rect.setTranslateX(180);

        // create ball
        ball.setFill(Color.BLUE);
        ball.setTranslateX(100);
        ball.setTranslateY(100);

        // setup timeline  duration is the complete cycle (100000 / 400 = 250 msec!)
        final Duration oneFrameAmt = Duration.millis(1000 / 60);
        final KeyFrame oneFrame = new KeyFrame(oneFrameAmt, e-> handleFrame());
        Timeline timeline = new Timeline();
        timeline.getKeyFrames().add(oneFrame);
        timeline.setCycleCount(Animation.INDEFINITE);
        timeline.play();

        // JavaFX boilerplate - setup stage
        Group grp_main = new Group();
        grp_main.getChildren().addAll(ball, rect);
        Scene scn_main = new Scene(grp_main, 400, 400);

        KeyStateManager km = new KeyStateManager(scn_main);
        km.setOnKeyChangeEvent( e -> handleKey(e.getKeyCode()) );

        stage.setScene(scn_main);
        stage.setResizable(false);
        stage.show();
    }

    private void handleKey(KeyCode key) {
        if (key == null) {
            currentDirection = DIRECTION.NONE;
            return;
        }

        switch(key) {
            case UP   : currentDirection = DIRECTION.UP;  break;
            case DOWN : currentDirection = DIRECTION.DOWN;  break;
            case LEFT : currentDirection = DIRECTION.LEFT;  break;
            case RIGHT: currentDirection = DIRECTION.RIGHT;  break;
            case SPACE : System.err.println("ACTION");  break;
            default : break;
        }
    }

    
    /**
     * Rationale:
     * <ul>
     *   <li>We only need to consider two quadrants (180°) - otherwise the
     *       ball moves away from the barrier</li>
     *   <li>The angle of the barrier is subtracted and then re-added</li>
     *   <li>For the reference (barrier) angle, we also only need to consider
     *       two quadrants (0 - 180°) - the value is properly calculated!  
     * </ul> 
     * @param old
     * @param reference
     * @return
     */
    private double calculateExitAngle(double old, double reference) {
        double normAngle = old - reference;
        if (normAngle < 0) {
            normAngle = normAngle + 2 * Math.PI;
        }

        double newAngle = 2*Math.PI - normAngle;
        return newAngle + reference;
    }


    private void handleFrame() {
        // update platform
        switch(currentDirection) {
            case NONE : 
                break;

            case LEFT :
                {
                    double pos = rect.getTranslateX();
                    if (pos > 0) {
                        rect.setTranslateX(pos - SPEED);
                    }
                }
                break;

            case RIGHT: 
                {
                    double pos = rect.getTranslateX();
                    if (pos < 350) {
                        rect.setTranslateX(pos + SPEED);
                    }
                }
                break;
        }

        // Update ball

        // check for collision
        Bounds bBall = ball.getBoundsInParent();
        Bounds bRect = rect.getBoundsInParent();
        if (bBall.intersects(bRect)) {
            double jitter = Math.random() * Math.PI / 180;
            bAngle = calculateExitAngle(bAngle,  0) + jitter;
        } else if (bBall.getMinY() < 0    ||
                   bBall.getMaxY() > 400) {
            bAngle = calculateExitAngle(bAngle,  0);
        } else if (bBall.getMinX() < 0 ||
                   bBall.getMaxX() > 400) {

            bAngle = calculateExitAngle(bAngle,  Math.PI/2);
        }

        double dx = bSpeed * Math.cos(bAngle);
        double dy = bSpeed * Math.sin(bAngle);

        ball.setTranslateX(ball.getTranslateX() + dx);
        ball.setTranslateY(ball.getTranslateY() - dy);

        // colorize ball depending on direction (up/down)
        if (bAngle > Math.PI){          // moving downwards
            ball.setFill(Color.GREEN);
        } else {                        // moving upwards
            ball.setFill(Color.BLUE);
        }
    }
}
