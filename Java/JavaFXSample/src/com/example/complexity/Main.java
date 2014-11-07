package com.example.complexity;

import java.util.ArrayList;
import java.util.List;

import com.example.Graph;

import javafx.application.Application;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.layout.GridPane;
import javafx.scene.paint.Color;
import javafx.stage.Stage;


public class Main extends Application {
   private Canvas canvas;
   private GraphicsContext context;

   private List<Graph> graphs = new ArrayList<Graph>();
  
   private double startX = 0;
   private double stopX = 1017;

   private double paddingLeft = 0;
   private double startY = 0;
   private double stopY = 0;

   private double x0pos = 0;
   private double y0pos = 0;
   private double tick = 0;
   private double scalePerPixel = 0;

   public static void main(String[] args) {
      launch(args);
   }


   
   private void drawYTick(double y) {
      long /*double*/ ypos = Math.round(y0pos - y / scalePerPixel);

      if (ypos > 20) { // do not draw a tick nearby the end of the axis!
         context.moveTo(x0pos - 5, ypos + 0.5);
         context.lineTo(x0pos + 5, ypos + 0.5);

         String value = "" + Math.round(y); //  * 10) / 10.0;
         double textWidth = 15; // / context.measureText(value).width;
         context.fillText(value, x0pos - textWidth - 7, ypos + 3);
      }
   }

   
   private void drawXTick(double x) {
      long /*double*/ xpos = Math.round(x0pos + x / scalePerPixel);

      context.moveTo(xpos + 0.5, y0pos - 5);
      context.lineTo(xpos + 0.5, y0pos + 5);

      String value = "" + Math.round(x); //  * 10) / 10.0;
      double textWidth = 15; // / FGV.context.measureText(value).width;
      context.fillText(value, xpos - textWidth / 2, y0pos + 15);
   }

   /**
    * Draws the axes including the ticks.
    */
   private void drawAxis() {

      // set the axis visual properties
      context.beginPath();
      context.setStroke(Color.BLACK);
      context.setLineWidth(1);
      // FGV.context.setLineDash([]);

      // draw the axis lines

      // Y axis
      context.moveTo(x0pos + 0.5, canvas.getHeight() + 0.5);
      context.lineTo(x0pos + 0.5, 0.5);

      // X axis
      context.moveTo(paddingLeft + 0.5, y0pos + 0.5);
      context.lineTo(canvas.getWidth() + 0.5, y0pos + 0.5);

      // draw the y scale
      for (double y = -tick; y > startY; y -= tick) { // negative axis
         drawYTick(y);
      }
      for (double y = tick; y < stopY; y += tick) { // positive axis
         drawYTick(y);
      }

      // draw the x scale
      for (double x = -tick; x > startX; x -= tick) { // negative axis
         drawXTick(x);
      }
      for (double x = tick; x < stopX; x += tick) { // positive axis
         drawXTick(x);
      }

      context.stroke();
   }
   

   private void drawGraph(Graph theGraph) {
       // do not draw the graph if it is in an error state
       if (theGraph.isError()) {
          return;
       }

       context.beginPath();
       context.setStroke(theGraph.getColor());
       context.setLineWidth(1);
       // FGV.context.setLineDash([]);

       boolean move = true;
      // theGraph.setT(this.t);
       for (double x = startX; x < stopX; x += scalePerPixel) {
          try {
             Double y = theGraph.evaluate(x);

             double xpos = x0pos + x / scalePerPixel;
             double ypos = y0pos - y / scalePerPixel;

             if (!Double.isNaN(y)) {
                if (move) {
                   move = false;
                   context.moveTo(xpos, ypos);
                } else {
                   context.lineTo(xpos, ypos);
                }
             } else {
                move = true;  // next non-NaN needs to MOVE first
             }
          } catch (Exception e) {
           //  legendTable.setError(theGraph.getIndex(), e.getLocalizedMessage());
             theGraph.setIsError(true);
             return;
          }
       }

       context.stroke();
    }

   /**
    * Redraws the complete canvas.
    */
   private void renderScene() {
      context.clearRect(0, 0, canvas.getWidth(), canvas.getHeight());

      // draw the axes
      drawAxis();

      // draw the graphs
      for (Graph graph : graphs) {
         drawGraph(graph);
      }
   }

   @Override
   public void start(Stage primaryStage) {
      primaryStage.setTitle("Simulation of JDK's container classes time complexity");

      final GridPane grid = new GridPane();
      grid.setAlignment(Pos.TOP_LEFT);
      // grid.setGridLinesVisible(true);

      Scene scene = new Scene(grid, 1200, 800);
      primaryStage.setScene(scene);

     // scene.getStylesheets().add
     //    (getClass().getResource("functions.css").toExternalForm());
     
      /* Setup the scene */
      canvas = new Canvas(640, 480);
      context = canvas.getGraphicsContext2D();

      double xRange = stopX - startX;

      // calculate graph parameters with the assumption that there is enough
      // space
      // for the y axis scale
      paddingLeft = 0;

      // auto-calculate the Y axis range
      startY = -(xRange * canvas.getHeight()) / (2 * canvas.getWidth());
      startY *= 2;
      stopY = -startY;

      scalePerPixel = (stopX - startX) / canvas.getWidth();
      // calculate the position of the origin point
      x0pos = -startX / scalePerPixel;
      y0pos = -startY / scalePerPixel;

      // recalculate graph parameters with a left padding if there is not enough
      // space for the y axis scale
      if (x0pos < 30) {
         // calculate graph parameters and reserve space for the y axis scale
         paddingLeft = 30 - x0pos;

         // auto-calculate the Y axis range
         startY = -(xRange * canvas.getHeight())
               / (2 * (canvas.getWidth() - paddingLeft));
         startY *= 2;
         startY += 50;
         
         // calculate the scale factor
         scalePerPixel = (stopX - startX) / (canvas.getWidth() - paddingLeft);

         // calculate the position of the origin point
         x0pos = -startX / scalePerPixel + paddingLeft;
         y0pos = -startY / scalePerPixel;
      }

      // calculate the distance for each tick
      tick = 59.98 * scalePerPixel;
      if (tick < 0.1) {
         tick = 0.1;
      } else if (tick < 0.5) {
         tick = 0.5;
      } else if (tick < 1.0) {
         tick = 1.0;
      }
      grid.add(canvas, 0, 0);

      Graph graph = new Graph("x^2", Color.GREEN);
      graphs.add(graph);
      graph = new Graph("log(x)", Color.RED);
      graphs.add(graph);
      graph = new Graph("x", Color.BLUE);
      graphs.add(graph);
      graph = new Graph("x * log(x)", Color.BLACK);
      graphs.add(graph);

      renderScene();
      
      primaryStage.show();
   }
}

