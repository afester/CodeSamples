package com.example.javafx;

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
    private Rectangle area = new Rectangle(10, 10, 390, 250);

    private DIRECTION currentDirection = DIRECTION.NONE;
    private final static double SPEED = 12.0;
    private final static double STARTANGLE_VARIANCE = Math.toRadians(45);   // +/- 45°

    // ball vector
    private double bAngle = Math.toRadians(300); // 315);
    private double bSpeed = 4.0;
    double dx = 0;
    double dy = 0;

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

        dx = bSpeed * Math.cos(bAngle);
        dy = bSpeed * Math.sin(bAngle);

        area.setFill(Color.LIGHTGRAY);
        area.setStroke(Color.BLACK);

        // create rectangle to move
        rect.setFill(Color.BLACK);
        rect.setTranslateY(200);
        rect.setTranslateX(180);

        // create ball
        resetBall();

        // setup timeline  duration is the complete cycle (100000 / 400 = 250 msec!)
        final Duration oneFrameAmt = Duration.millis(1000 / 60);
        final KeyFrame oneFrame = new KeyFrame(oneFrameAmt, e-> handleFrame());
        Timeline timeline = new Timeline();
        timeline.getKeyFrames().add(oneFrame);
        timeline.setCycleCount(Animation.INDEFINITE);
        timeline.play();

        // JavaFX boilerplate - setup stage
        Group grp_main = new Group();
        grp_main.getChildren().addAll(area, ball, rect);
        Scene scn_main = new Scene(grp_main, area.getWidth() + area.getX(),
                                             area.getHeight() + area.getY());

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

    private boolean isExploding = false;

    private void resetBall() {
        ball.setFill(Color.BLUE);
        ball.setTranslateX(area.getX() + area.getWidth() / 2);

        double min = Math.toRadians(270) - STARTANGLE_VARIANCE;
        double max = Math.toRadians(270) + STARTANGLE_VARIANCE;
        bAngle = Math.random() * (max - min) + min;
        dx = bSpeed * Math.cos(bAngle);
        dy = bSpeed * Math.sin(bAngle);

        // Note: translation point of circle is its middle point!
        ball.setTranslateY(area.getY() + ball.getRadius());
    }


    private Bounds bBall;
    private Bounds bRect;
    private Bounds bArea;

    private void handleFrame() {
        //long x1 = System.currentTimeMillis();

        // Update geometric bounds of all objects
        bBall = ball.getBoundsInParent();
        bRect = rect.getBoundsInParent();
        bArea = area.getBoundsInParent();

        handlePlatform();

        if (isExploding) {
            handleExplode();
        } else {
            advanceBall();
        }

        //long x2 = System.currentTimeMillis();
        //System.err.println(x2 - x1);
    }

    private void handlePlatform() {

        switch(currentDirection) {
            case NONE : 
                break;

            case LEFT :
                {
                    double newPos = rect.getTranslateX() - SPEED;
                    if (newPos < bArea.getMinX()) {
                        newPos = bArea.getMinX();
                    }
                    rect.setTranslateX(newPos);
                }
                break;
    
            case RIGHT: 
                {
                    double newPos = rect.getTranslateX() + SPEED;
                    if (newPos > bArea.getMaxX() - bRect.getWidth()) {
                        newPos = bArea.getMaxX() - bRect.getWidth();
                    }
                    rect.setTranslateX(newPos);
                }
                break;
        }
    }


    private void handleExplode() {
        if (bBall.getMinY() > bArea.getMaxY()) {
            isExploding = false;
            resetBall();
        } else {
            ball.setTranslateX(ball.getTranslateX() + dx);
            ball.setTranslateY(ball.getTranslateY() - dy);
        }
    }


    private void advanceBall() {
        double refAngle = 0;
        boolean doBounce = false;
        double jitter = 0;

        if (bBall.getMaxY() >= bRect.getMinY()) {
            if (bBall.getMaxX() < bRect.getMinX() ||
                bBall.getMinX() > bRect.getMaxX()) {
                isExploding = true;
                ball.setFill(Color.RED);
            } else {
                // calculate a value between -0.5 and 0.5 which corresponds to the
                // position within the platform where the ball bounces
                double pos = (ball.getTranslateX() - rect.getTranslateX()) / rect.getWidth();
                if (pos < 0) {pos = 0;};
                if (pos > 1) {pos = 1;};
                pos = pos - 0.5;

                System.err.printf("%s\n", pos);
                jitter = pos;
                doBounce = true;
            }
        }

        if (bBall.getMinY() <= bArea.getMinY() ||
            bBall.getMaxY() >= bArea.getMaxY()) {   // this must never happen!
            doBounce = true;
        } else if (bBall.getMinX() <= bArea.getMinX() ||
                   bBall.getMaxX() >= bArea.getMaxX()) {
            doBounce = true;
            refAngle = Math.PI/2;
        }

        if (doBounce) {
            bAngle = calculateExitAngle(bAngle, refAngle) + jitter;
            dx = bSpeed * Math.cos(bAngle);
            dy = bSpeed * Math.sin(bAngle);
        }

        ball.setTranslateX(ball.getTranslateX() + dx);
        ball.setTranslateY(ball.getTranslateY() - dy);
    }

}
