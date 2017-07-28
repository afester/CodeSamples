package com.example.javafx;

import javafx.application.Application;
import javafx.geometry.Point2D;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.SnapshotParameters;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.paint.Color;
import javafx.scene.paint.CycleMethod;
import javafx.scene.paint.ImagePattern;
import javafx.scene.paint.LinearGradient;
import javafx.scene.paint.Stop;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Line;
import javafx.scene.shape.Rectangle;
import javafx.scene.transform.Affine;
import javafx.scene.transform.NonInvertibleTransformException;
import javafx.scene.transform.Rotate;
import javafx.stage.Stage;

public class Gradient extends Application {

    public static void main(String[] args) {
        launch(args);
    }


    @Override
    public void start(Stage primaryStage) {
        primaryStage.setTitle("Sample application for JavaFX components");

        final Group mainGroup = new Group();

        Stop s1 = new Stop(0.0, Color.GREEN);
        Stop s2 = new Stop(1.0, Color.YELLOW);
        LinearGradient lg = new LinearGradient(50, 75, 250, 75, false, CycleMethod.NO_CYCLE, s1, s2);

        Point2D center = new Point2D(150, 100);
        Line gradLine = new Line(50, 100, 250, 100);
        gradLine.setStroke(Color.RED);
        Rectangle r = new Rectangle(50, 50, 200, 100);
        Circle pivot = new Circle(center.getX(), center.getY(), 2);

        // transform the object - the gradient is being transformed together with the object! 
        Affine a = new Affine();
        a.appendRotation(45, center);
        a.appendScale(2.0, 2.0);
        a.appendTranslation(100, -150);
        a.appendShear(0.4,  0.2);
        r.getTransforms().add(a);
        
        r.setFill(lg);
/********************************/
        Rectangle r2 = new Rectangle(50, 50, 200, 100);
        Affine a2 = a.clone();
        try {
            a2.invert();
        } catch (NonInvertibleTransformException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        
        System.err.println(r2.getBoundsInLocal());

        r2.getTransforms().add(a2);
        r2.setFill(lg);
        r2.getTransforms().add(a);
/********************************/

        Affine ac = new Affine();
        ac.appendRotation(45);
        ac.appendTranslation(-200,  -250);
        ac.appendShear(0.5, 0);
        ac.appendScale(2, 2);

    /****/
        LinearGradient lg2 = new LinearGradient(50, 75, 250, 75, false, CycleMethod.NO_CYCLE, 
                new Stop(0, Color.BLUE), new Stop(1, Color.RED));
        final Canvas canvas = new Canvas(250,250);
        GraphicsContext gc = canvas.getGraphicsContext2D();
        gc.transform(ac);
        gc.setFill(lg2);
        gc.fillRect(75, 75, 100, 100);

        SnapshotParameters params = new SnapshotParameters();
        Image result = canvas.snapshot(params, null);

        ImagePattern ip = new ImagePattern(result);
    /****/
///        gc.setStroke(Color.BLACK);
//        gc.rect(0,  0,  250,  250);
//        gc.stroke();
        r2.setFill(ip);

        mainGroup.getChildren().addAll(r, r2, pivot, gradLine); // , new ImageView(result));

        Scene scene = new Scene(mainGroup, 800, 600);
        primaryStage.setScene(scene);
        primaryStage.show();
    }
}
