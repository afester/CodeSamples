package com.example;

import java.util.ArrayList;
import java.util.List;

import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.layout.GridPane;
import javafx.scene.paint.Color;
import javafx.scene.text.Text;
import javafx.stage.Stage;

import javax.script.ScriptEngine;
import javax.script.ScriptEngineManager;
import javax.script.ScriptException;


public class Functions extends Application {

   private List<Graph> graphs = new ArrayList<Graph>();
   
   private double startX = 0;
   private double stopX = 0;
   private double paddingLeft = 0;
   private double startY = 0;
   private double stopY = 0;
   private double scalePerPixel = 0;
   private double x0pos = 0;
   private double y0pos = 0;
   private double tick = 0;
   private double t = 1;

   private Canvas canvas;
   private GraphicsContext context;
   private TextField fromX;
   private TextField toX;
   private Label fromY;
   private Label toY;

   private LegendTable legendTable = new LegendTable(this);


   public static void main(String[] args) {
      launch(args);
   }


   
   @Override
   public void start(Stage primaryStage) {
      primaryStage.setTitle("Function graph plotter");

      final GridPane grid = new GridPane();
      grid.setAlignment(Pos.TOP_LEFT);
      // grid.setGridLinesVisible(true);
      
      Scene scene = new Scene(grid, 1200, 600);
      primaryStage.setScene(scene);

      scene.getStylesheets().add
         (getClass().getResource("functions.css").toExternalForm());
     
      /* Setup the scene */
      canvas = new Canvas(640, 480);
      context = canvas.getGraphicsContext2D();
      context.setFill(Color.BLUE);
      context.fillRect(75, 75, 100, 100);
      grid.add(canvas, 0, 0);

      final GridPane axisControlGrid = new GridPane();

      Label lbl = new Label("X axis:");
      axisControlGrid.add(lbl, 0, 1);
      fromX = new TextField();
      fromX.setText("-3.15");
      axisControlGrid.add(fromX, 1, 1);
      Label lbl2 = new Label("to");
      axisControlGrid.add(lbl2, 2, 1);
      toX = new TextField();
      toX.setText("6.3");
      axisControlGrid.add(toX, 3, 1);
      final Button btn = new Button();
      btn.setText("Apply");
      btn.setOnAction(new EventHandler<ActionEvent>() {

         @Override
         public void handle(ActionEvent arg0) {
            updateValues();
            renderScene();
         }
         
      });

      axisControlGrid.add(btn, 4, 1);

      Label lbl3 = new Label("Y axis:");
      axisControlGrid.add(lbl3, 0, 2);
      fromY = new Label();
      axisControlGrid.add(fromY, 1, 2);
      Label lbl4 = new Label("to");
      axisControlGrid.add(lbl4, 2, 2);
      toY = new Label();
      axisControlGrid.add(toY, 3, 2);
      Text txt = new Text("(calculated from X axis)");
      axisControlGrid.add(txt, 4, 2);

      grid.add(axisControlGrid, 0, 1);

      GridPane newDeleteButtons = new GridPane();
      Button addButton = new Button("Add");
      addButton.setOnAction(new EventHandler<ActionEvent>() {

         @Override
         public void handle(ActionEvent arg0) {
            addGraph("", Color.AQUA);

            // set row into edit mode
            legendTable.setRowEdit(graphs.size());
         }
      });

      newDeleteButtons.add(addButton, 0, 0);
      Button clearAllButton = new Button("Clear all");
      clearAllButton.setOnAction(new EventHandler<ActionEvent>() {

         @Override
         public void handle(ActionEvent arg0) {
            legendTable.removeLegend();
            graphs.clear();
            renderScene();
         }
      });
      newDeleteButtons.add(clearAllButton, 1, 0);

      GridPane editGrid = new GridPane();
      editGrid.add(legendTable, 0, 0);
      editGrid.add(newDeleteButtons, 0, 1);

      grid.add(editGrid, 1, 0);

      /* Add some sample functions */
      addGraph("2*Math.sin(x+2*t)", Color.RED);
      addGraph("Math.log(X)+t", Color.BLUE);
      addGraph("Math.exp(x+t)", Color.GREEN);
      addGraph("x + 3*Math.pow(x-t, 2) + Math.pow(x, 3) - 1", Color.MAROON);

      updateValues();
      renderScene();

      SceneGraphTree tree = new SceneGraphTree();
      tree.setSceneTree(scene);
      tree.show();

      primaryStage.show();
   }

   private void updateValues() {
      // get the x axis range as entered by the user
      startX = Double.parseDouble(fromX.getText());
      stopX = Double.parseDouble(toX.getText());
      double xRange = stopX - startX;

      // calculate graph parameters with the assumption that there is enough
      // space
      // for the y axis scale
      paddingLeft = 0;

      // auto-calculate the Y axis range
      startY = -(xRange * canvas.getHeight()) / (2 * canvas.getWidth());
      stopY = -startY;

      // calculate the scale factor
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
         stopY = -startY;

         // calculate the scale factor
         scalePerPixel = (stopX - startX) / (canvas.getWidth() - paddingLeft);

         // calculate the position of the origin point
         x0pos = -startX / scalePerPixel + paddingLeft;
         y0pos = -startY / scalePerPixel;
      }

      // update Y range scale as shown on the UI
      fromY.setText("" + Math.round(startY * 100) / 100.0);
      toY.setText("" + Math.round(stopY * 100) / 100.0);

      // calculate the distance for each tick
      tick = 20 * scalePerPixel;
      if (tick < 0.1) {
         tick = 0.1;
      } else if (tick < 0.5) {
         tick = 0.5;
      } else if (tick < 1.0) {
         tick = 1.0;
      }
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
      context.moveTo(x0pos, canvas.getHeight());
      context.lineTo(x0pos, 0);

      // X axis
      context.moveTo(paddingLeft, y0pos);
      context.lineTo(canvas.getWidth(), y0pos);

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

      /*
       * // Y axis arrow FGV.context.moveTo(FGV.x0pos, 0);
       * FGV.context.lineTo(FGV.x0pos - 5, 10); FGV.context.lineTo(FGV.x0pos +
       * 5, 10); FGV.context.lineTo(FGV.x0pos, 0); FGV.context.strokeText("Y",
       * FGV.x0pos + FGV.paddingLeft - 15, 10);
       * 
       * // X axis arrow FGV.context.moveTo(FGV.canvas.width, FGV.y0pos);
       * FGV.context.lineTo(FGV.canvas.width - 10, FGV.y0pos - 5);
       * FGV.context.lineTo(FGV.canvas.width - 10, FGV.y0pos + 5);
       * FGV.context.lineTo(FGV.canvas.width, FGV.y0pos);
       * FGV.context.strokeText("X", FGV.canvas.width - 10, FGV.y0pos + 20);
       * 
       * FGV.context.fill();
       */
   }

   
   private void drawYTick(double y) {
      double ypos = y0pos - y / scalePerPixel;

      if (ypos > 20) { // do not draw a tick nearby the end of the axis!
         context.moveTo(x0pos - 5, ypos);
         context.lineTo(x0pos + 5, ypos);

         String value = "" + Math.round(y * 10) / 10.0;
         double textWidth = 20; // / context.measureText(value).width;
         context.fillText(value, x0pos - textWidth - 7, ypos + 3);
      }
   }

   
   private void drawXTick(double x) {
      double xpos = x0pos + x / scalePerPixel;

      context.moveTo(xpos, y0pos - 5);
      context.lineTo(xpos, y0pos + 5);

      String value = "" + Math.round(x * 10) / 10.0;
      double textWidth = 20; // / FGV.context.measureText(value).width;
      context.fillText(value, xpos - textWidth / 2, y0pos + 15);
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

      /*
       * // draw the crosshair if (FGV.mouseXpos !== -1 && FGV.mouseYpos !== -1)
       * { // set visual properties for the cross FGV.context.beginPath();
       * FGV.context.strokeStyle = "red"; FGV.context.lineWidth = 0.5;
       * FGV.context.setLineDash([ 8, 2 ]);
       * 
       * // draw the cross FGV.context.moveTo(FGV.mouseXpos, 0);
       * FGV.context.lineTo(FGV.mouseXpos, FGV.canvas.height);
       * FGV.context.moveTo(0, FGV.mouseYpos);
       * FGV.context.lineTo(FGV.canvas.width, FGV.mouseYpos);
       * FGV.context.stroke();
       * 
       * // update position values FGV.curXposNode.value = Math
       * .round(((FGV.mouseXpos - FGV.x0pos) * FGV.scalePerPixel) * 100) / 100;
       * FGV.curYposNode.value = Math .round(((FGV.y0pos - FGV.mouseYpos) *
       * FGV.scalePerPixel) * 100) / 100; } else { FGV.curXposNode.value =
       * 'n/a'; FGV.curYposNode.value = 'n/a'; }
       */
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

      // draw the graph
      double x = startX;
      double t = this.t; // time parameter - for parametric functions
      double y = 0;
      try {
         y = eval(theGraph.getFormula(), x);
      } catch (ScriptException e) {
         legendTable.setError(theGraph.getIndex(), e.getLocalizedMessage());
         theGraph.setIsError(true);
         return;
      }
      // if (y === Infinity) {
      // y = 1000;
      // }

      double xpos = x0pos + x / scalePerPixel;
      double ypos = y0pos - y / scalePerPixel;
      context.moveTo(xpos, ypos);

      x += scalePerPixel;
      for (; x < stopX; x += scalePerPixel) {
         try {
            y = eval(theGraph.getFormula(), x);
         } catch (ScriptException e) {
            legendTable.setError(theGraph.getIndex(), e.getLocalizedMessage());
            theGraph.setIsError(true);
            return;
         }

         xpos = x0pos + x / scalePerPixel;
         ypos = y0pos - y / scalePerPixel;
         context.lineTo(xpos, ypos);
      }

      context.stroke();
   }

   
   private void addGraph(String func, Color col) {
      Graph graph = new Graph(func, col);
      graphs.add(graph);
      graph.setIndex(graphs.size());
      legendTable.addLegendEntry(graph);
   }


   public void setGraph(int i, String newFormula, Color newColor) {
      Graph graph = graphs.get(i);
      graph.setFormula(newFormula);
      graph.setColor(newColor);
      graph.setIsError(false);
      renderScene();
   }


   private ScriptEngineManager manager = new ScriptEngineManager();
   private ScriptEngine engine = manager
         .getEngineByMimeType("application/javascript");

   private double eval(String formula, double x) throws ScriptException {
      engine.put("x", x);
      engine.put("t", 1);
      engine.eval("y = " + formula);
      Double y = (Double) engine.get("y");
      return y.doubleValue();
   }
}
