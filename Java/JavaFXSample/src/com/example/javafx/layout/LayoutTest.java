package com.example.javafx.layout;

import javafx.application.Application;
import javafx.collections.ObservableList;
import javafx.collections.ObservableSet;
import javafx.css.PseudoClass;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.ScrollPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Pane;
import javafx.scene.text.Text;
import javafx.stage.Stage;


class ObjectLayout extends Pane {

    private double horizontalSpacer = 5.0;

    @Override
    protected double computePrefWidth(double height) {
        System.err.println("computePrefWidth");

        double result = 0;
        for (Node child : getChildren()) {
            result += horizontalSpacer + child.prefWidth(-1);
        }

        System.err.println("   >" + result);
        return result; // super.computePrefWidth(height);
    }

    @Override
    protected double computePrefHeight(double width) {
        System.err.println("computePrefHeight");

        for (Node child : getChildren()) {
            System.err.println("  >" + child.getLayoutBounds());
        }

        return super.computePrefHeight(width);
    }


    @Override
    protected void layoutChildren() {
        System.err.println("layoutChildren");

//        System.err.println( ((Pane) getChildren().get(0)).getPrefWidth());
        super.layoutChildren();
//        getChildren().get(0).autosize();
//        //double w = computeChildMinAreaWidth(getChildren().get(0), 1.0, new Insets(0), 10, true);
//
//        System.err.println( ((Pane) getChildren().get(0)).getPrefWidth());
        double xpos = horizontalSpacer;
        for (Node child : getChildren()) {
            System.err.println("  >" + child.getLayoutBounds());
            child.setLayoutX(xpos);
            xpos += child.getLayoutBounds().getWidth() + horizontalSpacer;
        }

    }
//
//    public void update() {
//        System.err.println("update");
//
//        // update the layout bounds for all children
//        Scene dummy = new Scene(this);
////        // dummy.setUserAgentStylesheet(url);
////
//        this.applyCss();
//        this.layout();
//
//        for (Node child : getChildren()) {
//            System.err.println(child.getLayoutBounds());
//        }
//
//    }
}


// Simple node where the size is yet unknown
class RectDef extends Pane {

    public RectDef(String text) {
        getChildren().add(new Text(0, 15, text));

        setStyle("-fx-background-color: #C0FFD0; -fx-border-style: solid;");
        setLayoutX(1.0);
        setLayoutY(2.0);
    }
    
    
}


class MyHBox extends HBox {

    @Override
    protected double computeMinWidth(double height) {
        System.err.println("computeMinWidth");
        return super.computeMinWidth(height);
    }

    @Override
    protected double computeMinHeight(double width) {
        System.err.println("computeMinHeight");
        return super.computeMinHeight(width);
    }

    @Override
    protected double computePrefWidth(double height) {
        for (Node child : getChildren()) {
            System.err.println("  1>" + child.prefWidth(-1) + " / " +  child.getLayoutBounds());
        }

        double result = super.computePrefWidth(height);

        for (Node child : getChildren()) {
            System.err.println("  2>" + child.prefWidth(-1) + " / " +  child.getLayoutBounds());
        }

        System.err.printf("computePrefWidth() = %s%n", result);
        return result;
    }

    @Override
    protected double computePrefHeight(double width) {
        System.err.println("computePrefHeight");
        return super.computePrefHeight(width);
    }

    @Override
    protected void layoutChildren() {
        System.err.println("layoutChildren");
        super.layoutChildren();
    }
    
}


public class LayoutTest extends Application {


     @Override
     public void start(Stage primaryStage) throws Exception {
         primaryStage.setTitle("Layout test");

         ObjectLayout ol = new ObjectLayout();
         //HBox ol = new MyHBox();

         RectDef rd1 = new RectDef("Hello");
         RectDef rd2 = new RectDef("World");
         RectDef rd3 = new RectDef("how are you?");
         RectDef rd4 = new RectDef("123");

         ol.getChildren().addAll(rd1, rd2, rd3, rd4);

       //  ol.update();

         ScrollPane sp = new ScrollPane();
         sp.setContent(ol);

         System.err.println("Creating Scene ...");
         Scene scene = new Scene(sp, 100, 100);
         primaryStage.setScene(scene);
         primaryStage.show();
     }

     
     public static void main(String[] args) {
         launch(args);
     }
}
