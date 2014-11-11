package com.example.complexity;

import javafx.application.Application;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.layout.GridPane;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

import com.example.Graph;
import com.example.graph.GraphCanvas;
import com.example.graph.GraphParameters;


public class Main extends Application {
    private GraphCanvas canvas;


    public static void main(String[] args) {
      launch(args);
   }

   @Override
   public void start(Stage primaryStage) {
      primaryStage.setTitle("Simulation of JDK's container classes time complexity");

      final GridPane grid = new GridPane();
      grid.setAlignment(Pos.TOP_LEFT);
      grid.setGridLinesVisible(true);

      Scene scene = new Scene(grid, 1200, 800);
      primaryStage.setScene(scene);

     // scene.getStylesheets().add
     //    (getClass().getResource("functions.css").toExternalForm());

      GraphParameters gp = new GraphParameters(640, 480, 20, 20, 20, 20,
              -5, 5, //                                                //-50.0, 200.0, 
                                               0.75, 40);
      canvas = new GraphCanvas(gp);
      grid.add(canvas, 0, 0);

      canvas.addGraph(new Graph("x^2", Color.GREEN));
      canvas.addGraph(new Graph("log(x)", Color.RED));
      canvas.addGraph(new Graph("x * log(x)", Color.BLACK));

      canvas.renderScene();

      primaryStage.show();
   }
}
