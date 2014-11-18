package com.example.javafx.components;

import javafx.animation.Interpolator;
import javafx.animation.Timeline;
import javafx.animation.TranslateTransition;
import javafx.application.Application;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.event.Event;
import javafx.event.EventHandler;
import javafx.geometry.Pos;
import javafx.scene.Group;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.CheckBox;
import javafx.scene.control.Slider;
import javafx.scene.layout.GridPane;
import javafx.scene.paint.Color;
import javafx.scene.paint.CycleMethod;
import javafx.scene.paint.LinearGradient;
import javafx.scene.paint.Stop;
import javafx.scene.shape.Arc;
import javafx.scene.shape.Ellipse;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Text;
import javafx.scene.transform.Shear;
import javafx.scene.transform.Transform;
import javafx.stage.Stage;
import javafx.util.Duration;


public class Main extends Application {

    public static void main(String[] args) {
      launch(args);
   }

   @Override
   public void start(Stage primaryStage) {
      primaryStage.setTitle("Sample application for JavaFX components");

      final Group mainGroup = new Group();

      final GridPane grid = new GridPane();
      grid.setAlignment(Pos.TOP_LEFT);
      // grid.setGridLinesVisible(true);

      Text textRef = new Text("Hello World");
      grid.add(new Text("Text:"), 0, 0);
      grid.add(textRef, 1, 0);

      Button but = new Button("Ok");
      grid.add(new Text("Button:"), 0, 1);
      grid.add(but, 1, 1);

      final Group g1 = new Group();
      g1.getTransforms().add(new Shear(-0.5, 0.0));
      Text t2 = new Text("Text in Group");
      Button b2 = new Button("Button in Group");
      g1.getChildren().add(t2);
      g1.getChildren().add(b2);
      grid.add(new Text("Group"), 0, 2);
      grid.add(g1, 1, 2);
      final CheckBox c1 = new CheckBox("Toggle group");
      c1.setSelected(true);
      c1.setOnAction(new EventHandler<ActionEvent>() {

        @Override
        public void handle(ActionEvent arg0) {
            g1.setVisible(c1.isSelected());
        }
      });
      grid.add(c1, 2, 2);

      Slider s1 = new Slider(0.0, 1.0, 1.0);
      s1.valueProperty().addListener(/*(val, oldVal, newVal) -> {
                                          g1.setOpacity(newVal.doubleValue());
                                      });*/
              
              new ChangeListener<Number>() {

        @Override
        public void changed(ObservableValue<? extends Number> arg0,
                            Number oldValue, Number newValue) {
            g1.setOpacity(newValue.doubleValue());
        }
      });

      grid.add(s1, 3, 2);

      Stop[] stops = new Stop[]{new Stop(0, Color.web("0x4EBBCC")), 
              new Stop(1, Color.web("0x6D84A3"))};
      LinearGradient linearGradient = new LinearGradient(0, 0, 0, 1, true, CycleMethod.NO_CYCLE, stops);

      Rectangle r1 = new Rectangle(100, 100);
      r1.setFill(linearGradient);

      Ellipse e1 = new Ellipse(30, 80);
      e1.setRotate(45);
      e1.setFill(linearGradient);

      Arc a1 = new Arc(100, 100, 30, 80, 45, 200);
      a1.setFill(linearGradient);

      grid.add(new Text("Shapes:"), 0, 3);
      grid.add(r1, 1, 3);
      grid.add(e1, 2, 3);
      grid.add(a1, 3, 3);

/********** Simple animation sample */      
      
      final Button b3 = new Button("Absolute");
      b3.translateXProperty().addListener(new ChangeListener<Number>() {

        @Override
        public void changed(ObservableValue<? extends Number> arg0,
                            Number oldValue, Number newValue) {
            x = newValue.doubleValue();
            b3.setText(String.format("Catch me if you can (%.1f/%.1f)", x, y));
        }
      });
      b3.translateYProperty().addListener(new ChangeListener<Number>() {

          @Override
          public void changed(ObservableValue<? extends Number> arg0,
                              Number oldValue, Number newValue) {
              y = newValue.doubleValue();
              b3.setText(String.format("Catch me if you can (%.1f/%.1f)", x, y));
          }
      });

      b3.setLayoutX(300);
      b3.setLayoutY(400);
      
      // Provides the animated scrolling behavior for the text
      TranslateTransition transTransition = new TranslateTransition(new Duration(75000), b3);
      transTransition.setToY(-820);
      transTransition.setToX(-820);
      transTransition.setInterpolator(Interpolator.LINEAR);
      transTransition.setCycleCount(Timeline.INDEFINITE);

      mainGroup.getChildren().add(grid);
      mainGroup.getChildren().add(b3);
      
      
      
      
      
      Scene scene = new Scene(mainGroup, 800, 600);
      primaryStage.setScene(scene);

      primaryStage.show();

      transTransition.play();
   }
   
   private double x=0, y=0;

}