package com.example;

import java.util.ArrayList;
import java.util.List;

import com.example.graph.FunctionGraph;

import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.GridPane;
import javafx.scene.paint.Color;
import javafx.scene.text.Text;

public class LegendTable extends GridPane  {


   private Image pencilImage;
   private Image deleteImage;
   private Image checkImage;
   private Image cancelImage;

   private List<Button> editButtons = new ArrayList<>();
   private List<Button> checkButtons = new ArrayList<>();
   private List<Button> deleteButtons = new ArrayList<>();
   private List<Button> cancelButtons = new ArrayList<>();
   private List<Label> formulaTexts = new ArrayList<>();
   private List<TextField> formulas = new ArrayList<>();
   private List<Label> errorLabels = new ArrayList<>();

   private Functions plotter;

   public LegendTable(Functions plotter) {
      pencilImage = new Image(getClass().getResourceAsStream("pencil.png"));
      deleteImage = new Image(getClass().getResourceAsStream("delete.png"));
      checkImage = new Image(getClass().getResourceAsStream("check_mark.png"));
      cancelImage = new Image(getClass().getResourceAsStream("cancel.png"));
      add(new Text("Function Graphs:"), 0, 0, 4, 1);
      // setGridLinesVisible(true);
      this.plotter = plotter;
   }

   
   public int getSize() {
       return formulas.size();
   }

   /**
    * Adds a new row to the legend grid.
    * @param graph
    */
   public void addLegendEntry(FunctionGraph graph) {
      final int row = formulas.size();

      Button b1 = new Button();
      b1.setGraphic(new ImageView(pencilImage));
      b1.setOnAction(new EventHandler<ActionEvent>() {

         @Override
         public void handle(ActionEvent arg0) {
            setRowEdit(row);
         }
      });
      editButtons.add(b1);

      Button b2 = new Button();
      b2.setId("" + b2.hashCode());
      b2.setGraphic(new ImageView(checkImage));
      b2.setVisible(false);
      b2.setOnAction(new EventHandler<ActionEvent>() {

         @Override
         public void handle(ActionEvent arg0) {
            System.err.println("commit row " + row );
            String newFormula = formulas.get(row).getText();
            //System.err.println("  formula: " + newFormula);
            Color newColor = Color.BLUEVIOLET;  // TODO: read from drop down

            // set new values
            formulaTexts.get(row).setText(String.format("f%d(x) = %s", row, newFormula));
            plotter.setGraph(row, newFormula, newColor);

            setRowNoEdit(row);
         }
      });
      checkButtons.add(b2);

      Button b3 = new Button();
      b3.setGraphic(new ImageView(deleteImage));
      b3.setOnAction(new EventHandler<ActionEvent>() {

         @Override
         public void handle(ActionEvent arg0) {
            System.err.println("Deleting row " + row);
            removeLegendEntry(row);
            plotter.removeGraph(row);
         }

      });
      deleteButtons.add(b3);

      Button b4 = new Button();
      b4.setGraphic(new ImageView(cancelImage));
      b4.setVisible(false);
      b4.setOnAction(new EventHandler<ActionEvent>() {

         @Override
         public void handle(ActionEvent arg0) {
            setRowNoEdit(row);
            // renderScene();
         }
      });
      cancelButtons.add(b4);

      TextField formula = new TextField(graph.getFormula());
      formula.setVisible(false);
      formulas.add(formula);
      Label formulaText = new Label(String.format("f%d(x) = %s", row, graph.getFormula()));
      formulaTexts.add(formulaText);

      Label errorLabel = new Label();
      errorLabel.setTextFill(Color.RED);
      errorLabels.add(errorLabel);

      add(b1, 0, row+1);
      add(b2, 0, row+1);
      add(b3, 1, row+1);
      add(b4, 1, row+1);
      add(formula, 2, row+1);
      add(formulaText, 2, row+1);
      add(errorLabel, 3, row+1);

      // setGridLinesVisible(true);
   }

   private void removeLegendEntry(int row) {
       getChildren().remove(editButtons.get(row));
       getChildren().remove(checkButtons.get(row));
       getChildren().remove(deleteButtons.get(row));
       getChildren().remove(cancelButtons.get(row));
       getChildren().remove(formulaTexts.get(row));
       getChildren().remove(formulas.get(row));
       getChildren().remove(errorLabels.get(row));
   }

   public void removeLegend() {
      getChildren().clear();
      editButtons.clear();
      checkButtons.clear();
      deleteButtons.clear();
      cancelButtons.clear();
      formulaTexts.clear();
      formulas.clear();
      errorLabels.clear();
   }


   public void setRowNoEdit(int i) {
      System.err.println("NoEditing row " + i);

      editButtons.get(i).setVisible(true);
      checkButtons.get(i).setVisible(false);
      deleteButtons.get(i).setVisible(true);
      cancelButtons.get(i).setVisible(false);
      formulaTexts.get(i).setVisible(true);
      formulas.get(i).setVisible(false);
      errorLabels.get(i).setText("");     // also done here in the javascript version,
                                          // but not 100% correct since the error 
                                          // must be retained when cancelling
   }

   public void setRowEdit(int i) {
      System.err.println("Editing row " + i);

      editButtons.get(i).setVisible(false);
      checkButtons.get(i).setVisible(true);
      deleteButtons.get(i).setVisible(false);
      cancelButtons.get(i).setVisible(true);
      formulaTexts.get(i).setVisible(false);
      formulas.get(i).setVisible(true);
   }

   public void setError(int index, String s) {
      index--;

      //Matcher matcher = pattern.matcher(s);
      //matcher.find();
      errorLabels.get(index).setText(s); // "Error: " + matcher.group(1));
   }
}