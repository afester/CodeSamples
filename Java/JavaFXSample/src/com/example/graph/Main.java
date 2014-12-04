package com.example.graph;

import javafx.application.Application;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.layout.GridPane;
import javafx.scene.paint.Color;
import javafx.stage.Stage;


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

      canvas.addGraph(new FunctionGraph("x^2"), Color.GREEN);
      canvas.addGraph(new FunctionGraph("log(x)"), Color.RED);
      canvas.addGraph(new FunctionGraph("x * log(x)"), Color.BLACK);
      canvas.addGraph(new FunctionGraph("sqrt(x)"), Color.BLUE);
      canvas.addGraph(new DiscreteGraph(0.0, 0.5,
                  new double[] {1.0, 2.0, 3.0, 4.5,   5,   5,   5,   5,
                                4.5, 3.5, 2.0, 1.5, 0.5, 0.5, 0.5, 0.5,
                                1.0, 2.0, 3.0, 4.5,   5,   5,   5,   5,
                                4.5, 3.5, 2.0, 1.5, 0.5, 0.5, 0.5, 0.5,               
                                1.0, 2.0, 3.0, 4.5,   5,   5,   5,   5,
                                4.5, 3.5, 2.0, 1.5, 0.5, 0.5, 0.5, 0.5, 
                                1.0, 2.0, 3.0, 4.5,   5,   5,   5,   5,
                                4.5, 3.5, 2.0, 1.5, 0.5, 0.5, 0.5, 0.5,
                                1.0, 2.0, 3.0, 4.5,   5,   5,   5,   5,
                                4.5, 3.5, 2.0, 1.5, 0.5, 0.5, 0.5, 0.5}),
                                Color.AQUAMARINE);
      canvas.addGraph(new PointGraph(new double[][] {{-3.0, 2.0}, 
                                                     {2.0, 4.5},
                                                     {1.3, 4.5} }), Color.DEEPSKYBLUE);

      canvas.renderScene();

      primaryStage.show();
   }
}
