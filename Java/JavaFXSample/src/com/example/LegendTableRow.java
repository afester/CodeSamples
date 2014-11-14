package com.example;

import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.paint.Color;

import com.example.graph.FunctionGraph;

public class LegendTableRow  {

   private final static Image pencilImage = new Image(LegendTableRow.class.getResourceAsStream("pencil.png"));
   private final static Image deleteImage = new Image(LegendTableRow.class.getResourceAsStream("delete.png"));
   private final static Image checkImage = new Image(LegendTableRow.class.getResourceAsStream("check_mark.png"));
   private final static Image cancelImage = new Image(LegendTableRow.class.getResourceAsStream("cancel.png"));

   private Button editButton = null;
   private Button checkButton = null;
   private Button deleteButton = null;
   private Button cancelButton = null;
   private Label formulaText = null;
   private TextField formula = null;
   private Label errorLabel = null;
   private LegendTable theTable = null;

   public LegendTableRow(LegendTable table, final FunctionGraph graph) {
       final int row = table.getRowCount();       // TODO: Not well designed yet
       theTable = table;

       editButton = new Button();
       editButton.setGraphic(new ImageView(pencilImage));
       editButton.setOnAction(new EventHandler<ActionEvent>() {

          @Override
          public void handle(ActionEvent arg0) {
             System.err.println("EDIT");
             setEdit();
          }
       });

       checkButton = new Button();
       checkButton.setGraphic(new ImageView(checkImage));
       checkButton.setVisible(false);
       checkButton.setOnAction(new EventHandler<ActionEvent>() {

          @Override
          public void handle(ActionEvent arg0) {
             System.err.println("COMMIT");

             String newFormula = formula.getText();
             //System.err.println("  formula: " + newFormula);
             Color newColor = Color.BLUEVIOLET;  // TODO: read from drop down

             // set new values
             formulaText.setText(String.format("f%d(x) = %s", row, newFormula));
             theTable.commitRow(graph, newFormula, newColor);

             setNoEdit();
          }
       });
  
       deleteButton = new Button();
       deleteButton.setGraphic(new ImageView(deleteImage));
       deleteButton.setOnAction(new EventHandler<ActionEvent>() {

          @Override
          public void handle(ActionEvent arg0) {
             theTable.getChildren().remove(editButton);
             theTable.getChildren().remove(checkButton);
             theTable.getChildren().remove(deleteButton);
             theTable.getChildren().remove(cancelButton);
             theTable.getChildren().remove(formulaText);
             theTable.getChildren().remove(formula);
             theTable.getChildren().remove(errorLabel);

             theTable.deleteRow(graph);
          }

       });

       cancelButton = new Button();
       cancelButton.setGraphic(new ImageView(cancelImage));
       cancelButton.setVisible(false);
       cancelButton.setOnAction(new EventHandler<ActionEvent>() {

          @Override
          public void handle(ActionEvent arg0) {
              System.err.println("CANCEL");
              setNoEdit();
          }
       });


       formula = new TextField(graph.getFormula());
       formula.setVisible(false);

       formulaText = new Label(String.format("f%d(x) = %s", row, graph.getFormula()));

       Label errorLabel = new Label();
       errorLabel.setTextFill(Color.RED);

       theTable.add(editButton, 0, row+1);
       theTable.add(checkButton, 0, row+1);
       theTable.add(deleteButton, 1, row+1);
       theTable.add(cancelButton, 1, row+1);
       theTable.add(formulaText, 2, row+1);
       theTable.add(formula, 2, row+1);
       theTable.add(errorLabel, 3, row+1);
   }

   
   private void setNoEdit() {
      editButton.setVisible(true);
      checkButton.setVisible(false);
      deleteButton.setVisible(true);
      cancelButton.setVisible(false);
      formulaText.setVisible(true);
      formula.setVisible(false);
      errorLabel.setText("");     // also done here in the javascript version,
                                  // but not 100% correct since the error 
                                  // must be retained when cancelling
   }


   public void setEdit() {
      editButton.setVisible(false);
      checkButton.setVisible(true);
      deleteButton.setVisible(false);
      cancelButton.setVisible(true);
      formulaText.setVisible(false);
      formula.setVisible(true);
   }

/*
   public void setError(int index, String s) {
      index--;

      //Matcher matcher = pattern.matcher(s);
      //matcher.find();
      errorLabels.get(index).setText(s); // "Error: " + matcher.group(1));
   }*/
}