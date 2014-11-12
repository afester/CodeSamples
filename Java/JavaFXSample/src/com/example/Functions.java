package com.example;

import java.util.ArrayList;
import java.util.List;

import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
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
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.GridPane;
import javafx.scene.paint.Color;
import javafx.scene.text.Text;
import javafx.stage.Stage;
import javafx.util.Duration;


public class Functions extends Application {

   private List<FunctionGraph> graphs = new ArrayList<FunctionGraph>();
   
   private double startX = 0;
   private double stopX = 0;
   private double paddingLeft = 0;
   private double startY = 0;
   private double stopY = 0;
   private double scalePerPixel = 0;
   private double x0pos = 0;
   private double y0pos = 0;
   private double tick = 0;

   double mouseXpos = -1;
   double mouseYpos = -1;

   private double t = 1;
   private double dt = 0.1;
   private double startT = 0.0;
   private double stopT = 1.0;

   private int animationRate = 40;
   private Timeline stepTimeline = null;

   // UI components
   private Canvas canvas;
   private GraphicsContext context;
   private TextField fromX;
   private TextField toX;
   private Label fromY;
   private Label toY;
   private TextField curT;
   private TextField fromT;
   private TextField toT;
   private TextField curDT;

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

      Scene scene = new Scene(grid, 1200, 800);
      primaryStage.setScene(scene);

      scene.getStylesheets().add
         (getClass().getResource("functions.css").toExternalForm());
     
      /* Setup the scene */
      canvas = new Canvas(640, 480);
      context = canvas.getGraphicsContext2D();
      context.setFill(Color.BLUE);
      context.fillRect(75, 75, 100, 100);
      canvas.setOnMouseMoved(new EventHandler<MouseEvent>() {

         @Override
         public void handle(MouseEvent evt) {
            mouseXpos = evt.getX();
            mouseYpos = evt.getY();

            renderScene();
         }
         
      });
      canvas.setOnMouseExited(new EventHandler<MouseEvent>() {

         @Override
         public void handle(MouseEvent arg0) {
            mouseXpos = -1;
            mouseYpos = -1;

            renderScene();
         }
         
      });

      grid.add(canvas, 0, 0);

/********************************/
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
      final Button btn = new Button("Apply");
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

/********************************/
      
      final GridPane parameterControlGrid = new GridPane();
      Label lbl5 = new Label("t =");
      parameterControlGrid.add(lbl5,  0,  0);
      curT = new TextField();
      parameterControlGrid.add(curT,  1,  0);
      final Button applyParam = new Button("Apply");
      applyParam.setOnAction(new EventHandler<ActionEvent>() {

         @Override
         public void handle(ActionEvent arg0) {
            Functions.this.t = Double.parseDouble(curT.getText());
            renderScene();
         }
      });

      parameterControlGrid.add(applyParam,  2,  0);

      grid.add(parameterControlGrid, 0, 2);

/************************/

      final GridPane animationControlGrid = new GridPane();

      Label lbl6 = new Label("Animation:");
      animationControlGrid.add(lbl6,  0,  0);

      Label lbl7 = new Label("From t = ");
      animationControlGrid.add(lbl7,  1,  0);
      fromT = new TextField("" + startT);
      animationControlGrid.add(fromT,  2,  0);

      Label lbl8 = new Label("to");
      animationControlGrid.add(lbl8,  3,  0);
      toT = new TextField("" + stopT);
      animationControlGrid.add(toT,  4,  0);

      Label lbl9 = new Label("dt");
      animationControlGrid.add(lbl9,  5,  0);
      curDT = new TextField("" + dt);
      animationControlGrid.add(curDT,  6,  0);

      final Button startAnimation = new Button("Start");
      startAnimation.setOnAction(new EventHandler<ActionEvent>() {

         @Override
         public void handle(ActionEvent arg0) {
            if (stepTimeline != null) {
               return;
            }

            // read current values from the UI
            Functions.this.dt = Double.parseDouble(Functions.this.curDT.getText());
            Functions.this.startT = Double.parseDouble(Functions.this.fromT.getText());
            Functions.this.stopT = Double.parseDouble(Functions.this.toT.getText());

            Functions.this.t = Functions.this.startT;

            // Update UI
            String txt = String.valueOf(Math.round(Functions.this.t * 100) / 100.0);
            Functions.this.curT.setText(txt);
            renderScene();

            // schedule next steps
            // Duration (and KeyFrame) are immutable, and Timeline does not contain
            // a setter to set individual frames 
            stepTimeline = 
                  new Timeline(new KeyFrame(Duration.millis(animationRate), 
                        new EventHandler<ActionEvent>() {

               @Override
               public void handle(ActionEvent event) {
                  nextStep();
               }
           }));
           stepTimeline.setCycleCount(Timeline.INDEFINITE);
           stepTimeline.play();
         }
      });
      animationControlGrid.add(startAnimation,  7,  0);

      final Button stopAnimation = new Button("Stop");
      stopAnimation.setOnAction(new EventHandler<ActionEvent>() {

         @Override
         public void handle(ActionEvent arg0) {
            if (stepTimeline != null) {
               stepTimeline.stop();
               stepTimeline = null;
            }
         }
      });
      animationControlGrid.add(stopAnimation,  8,  0);

      final Button nextParam = new Button("+");
      nextParam.setOnAction(new EventHandler<ActionEvent>() {

         @Override
         public void handle(ActionEvent arg0) {
            // read current values from the UI
            Functions.this.dt = Double.parseDouble(Functions.this.curDT.getText());
            Functions.this.startT = Double.parseDouble(Functions.this.fromT.getText());
            Functions.this.stopT = Double.parseDouble(Functions.this.toT.getText());

            nextStep();
         }
      });
      animationControlGrid.add(nextParam,  9,  0);

      final Button prevParam = new Button("-");
      prevParam.setOnAction(new EventHandler<ActionEvent>() {

         @Override
         public void handle(ActionEvent arg0) {
            // read current values from the UI
            Functions.this.dt = Double.parseDouble(Functions.this.curDT.getText());
            Functions.this.startT = Double.parseDouble(Functions.this.fromT.getText());
            Functions.this.stopT = Double.parseDouble(Functions.this.toT.getText());

            previousStep();
         }
      });
      animationControlGrid.add(prevParam,  10,  0);

      grid.add(animationControlGrid, 0, 3);

/************************/

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
      //addGraph("2*sin(x+2*t)", Color.RED);
      //addGraph("log(x)+t", Color.BLUE);
      //addGraph("e^(X+t)", Color.GREEN);
      //addGraph("x + 3*(x-t)^2 + x^3 - 1", Color.MAROON);
      //addGraph("tan(x)", Color.BLUEVIOLET);
      //addGraph("sqrt(x)", Color.YELLOWGREEN);
      //addGraph("1/x", Color.AQUA);
      addGraph("sin(x) + t", Color.RED);

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
      long /*double*/ ypos = Math.round(y0pos - y / scalePerPixel);

      if (ypos > 20) { // do not draw a tick nearby the end of the axis!
         context.moveTo(x0pos - 5, ypos + 0.5);
         context.lineTo(x0pos + 5, ypos + 0.5);

         String value = "" + Math.round(y * 10) / 10.0;
         double textWidth = 20; // / context.measureText(value).width;
         context.fillText(value, x0pos - textWidth - 7, ypos + 3);
      }
   }

   
   private void drawXTick(double x) {
      long /*double*/ xpos = Math.round(x0pos + x / scalePerPixel);

      context.moveTo(xpos + 0.5, y0pos - 5);
      context.lineTo(xpos + 0.5, y0pos + 5);

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
      for (FunctionGraph graph : graphs) {
         drawGraph(graph);
      }


       // draw the crosshair 
       if (mouseXpos != -1 && mouseYpos != -1) {
          // set visual properties for the cross
          context.beginPath();
          context.setStroke(Color.RED);
          context.setLineWidth(0.5);
          // context.setLineDash([ 8, 2 ]);

          // draw the cross
          // NOTE: 
          //    https://forums.oracle.com/thread/2465226
          //    http://stackoverflow.com/questions/11881834/what-are-a-lines-exact-dimensions-in-javafx-2
          //    http://docs.oracle.com/javafx/2/api/javafx/scene/Node.html
          // A pixel has a dimension of 1.0 x 1.0 - the center of the pixel is
          // 0.5/0.5. In order to draw sharp lines, we need to transform the
          // coordinates to pixel centers
          context.moveTo(mouseXpos + 0.5, 0.5);
          context.lineTo(mouseXpos + 0.5, canvas.getHeight() + 0.5);
          context.moveTo(0.5, mouseYpos + 0.5);
          context.lineTo(canvas.getWidth() + 0.5, mouseYpos + 0.5);
          context.stroke();
 
          // update position values 
          // FGV.curXposNode.value = Math.round(((FGV.mouseXpos - FGV.x0pos) * FGV.scalePerPixel) * 100) / 100;
          // FGV.curYposNode.value = Math .round(((FGV.y0pos - FGV.mouseYpos) * FGV.scalePerPixel) * 100) / 100;
       } else { 
          // FGV.curXposNode.value = "n/a";
          // FGV.curYposNode.value = "n/a";
       }
   }

   
   private void drawGraph(FunctionGraph theGraph) {
      // do not draw the graph if it is in an error state
      if (theGraph.isError()) {
         return;
      }

      context.beginPath();
      context.setStroke(theGraph.getColor());
      context.setLineWidth(1);
      // FGV.context.setLineDash([]);

      boolean move = true;
      theGraph.setT(this.t);
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
            legendTable.setError(theGraph.getIndex(), e.getLocalizedMessage());
            theGraph.setIsError(true);
            return;
         }
      }

      context.stroke();
   }

   
   private void addGraph(String func, Color col) {
      FunctionGraph graph = new FunctionGraph(func, col);
      graphs.add(graph);
      graph.setIndex(graphs.size());
      legendTable.addLegendEntry(graph);
   }


   public void setGraph(int i, String newFormula, Color newColor) {
      FunctionGraph graph = graphs.get(i);
      graph.setFormula(newFormula);
      graph.setColor(newColor);
      graph.setIsError(false);
      renderScene();
   }



   private void nextStep() {
      // calculate next t
      this.t = this.t + this.dt;

      // check if value still in range - if end is reached, reset parameter
      if (this.dt < 0) {    // counting backwards
          if (this.t <= this.stopT) {
              // FGV.animate = false;
             this.t = this.startT;
          }
      } else {             // counting forward
          if (this.t >= this.stopT) {
              // FGV.animate = false;
             this.t = this.startT;
          }
      }

      // Update UI
      String txt = String.valueOf(Math.round(this.t * 100) / 100.0);
      this.curT.setText(txt);
      renderScene();
   }



   private void previousStep() {
      // calculate next t
      this.t = this.t - this.dt;

      // check if value still in range - if end is reached, reset parameter
      if (this.dt < 0) {    // counting backwards
          if (this.t > this.startT) {
             this.t = this.stopT;
          }
      } else {             // counting forward
          if (this.t <= this.startT) {
              // FGV.animate = false;
             this.t = this.stopT;
          }
      }

      // Update UI
      String txt = String.valueOf(Math.round(this.t * 100) / 100.0);
      this.curT.setText(txt);
      renderScene();
   }
}
