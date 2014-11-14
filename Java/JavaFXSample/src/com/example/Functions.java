package com.example;

import com.example.graph.FunctionGraph;
import com.example.graph.Graph;
import com.example.graph.GraphCanvas;
import com.example.graph.GraphParameters;

import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.layout.GridPane;
import javafx.scene.paint.Color;
import javafx.scene.text.Text;
import javafx.stage.Stage;
import javafx.util.Duration;


public class Functions extends Application {

   double mouseXpos = -1;
   double mouseYpos = -1;

   private double t = 1;
   private double dt = 0.1;
   private double startT = 0.0;
   private double stopT = 1.0;

   private int animationRate = 40;
   private Timeline stepTimeline = null;

   // UI components
   private GraphCanvas canvas;
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
      GraphParameters gp = new GraphParameters(640, 480, 20, 20, 20, 20,
                                               -1, 1, 0.5, 40);
      canvas = new GraphCanvas(gp);
      canvas.setAxisColor(Color.BLUE);
      grid.add(canvas, 0, 0);

/*
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
*/

/* Range *******************************/
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
            canvas.renderScene();
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

/* Parameter ******************************/
      
      final GridPane parameterControlGrid = new GridPane();
      Label lbl5 = new Label("t =");
      parameterControlGrid.add(lbl5,  0,  0);
      curT = new TextField();
      parameterControlGrid.add(curT,  1,  0);
      final Button applyParam = new Button("Apply");
      applyParam.setOnAction(new EventHandler<ActionEvent>() {

         @Override
         public void handle(ActionEvent arg0) {
            canvas.setT(Double.parseDouble(curT.getText()));
            canvas.renderScene();
         }
      });

      parameterControlGrid.add(applyParam,  2,  0);

      grid.add(parameterControlGrid, 0, 2);

/* Animation ***********************/

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
            canvas.renderScene();

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

/* Legend ***********************/

      GridPane newDeleteButtons = new GridPane();
      Button addButton = new Button("Add");
      addButton.setOnAction(new EventHandler<ActionEvent>() {

         @Override
         public void handle(ActionEvent arg0) {
            Graph newGraph = addGraph("", Color.AQUA);

            // set row into edit mode
            legendTable.setRowEdit(newGraph);
         }
      });
      newDeleteButtons.add(addButton, 0, 0);

      Button clearAllButton = new Button("Clear all");
      clearAllButton.setOnAction(new EventHandler<ActionEvent>() {

         @Override
         public void handle(ActionEvent arg0) {
            legendTable.removeLegend();
            canvas.clear();
            canvas.renderScene();
         }
      });
      newDeleteButtons.add(clearAllButton, 1, 0);

      GridPane editGrid = new GridPane();
      editGrid.add(legendTable, 0, 0);
      editGrid.add(newDeleteButtons, 0, 1);

      grid.add(editGrid, 1, 0);

/* ****************************************/
      /* Add some sample functions */
      //addGraph("2*sin(x+2*t)", Color.RED);
      //addGraph("log(x)+t", Color.BLUE);
      //addGraph("e^(X+t)", Color.GREEN);
      //addGraph("x + 3*(x-t)^2 + x^3 - 1", Color.MAROON);
      //addGraph("tan(x)", Color.BLUEVIOLET);
      //addGraph("sqrt(x)", Color.YELLOWGREEN);
      addGraph("1/x", Color.AQUA);
      addGraph("sin(x) + t", Color.RED);

      updateValues();
      canvas.renderScene();

      SceneGraphTree tree = new SceneGraphTree();
      tree.setSceneTree(scene);
      tree.show();

      primaryStage.show();
   }

   private void updateValues() {
      // get the x axis range as entered by the user
      double startX = Double.parseDouble(fromX.getText());
      double stopX = Double.parseDouble(toX.getText());
      canvas.setRange(startX, stopX);
   }

/*      
      
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
    * Redraws the complete canvas.
    *
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
   */

   private Graph addGraph(String func, Color col) {
      FunctionGraph graph = new FunctionGraph(func);
      canvas.addGraph(graph, col);
      legendTable.addLegendEntry(graph);
      return graph;
   }


   public void setGraph(FunctionGraph graph, String newFormula, Color newColor) {
      graph.setFormula(newFormula);
      graph.setIsError(false);
      canvas.setGraphColor(graph, newColor);

      canvas.renderScene();
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
      
      canvas.setT(this.t);
      canvas.renderScene();
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

      canvas.setT(this.t);
      canvas.renderScene();
   }

    public void removeGraph(Graph graph) {
        canvas.removeGraph(graph);
        canvas.renderScene();
    }

}
