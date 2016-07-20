package com.example.tree;

import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Line;
import javafx.scene.text.Text;

class LayoutDataImpl<T> implements LayoutData<T> {
    int yCoordinate;
    float xCoordinate;
    float flPrelim;
    float flModifier; // mod

    TreeNode<T> prev; // left neighbor
    

    Pane nodePanel;     // reference of the JavaFX scene node which renders this tree node
    Line edgeLine;

    public void createNodePanel(TreeNode<T> node) {
        nodePanel = new Pane();
        nodePanel.setLayoutX(xCoordinate);
        nodePanel.setLayoutY(yCoordinate);
        nodePanel.getStyleClass().add("TreeNode");
        nodePanel.getStyleClass().add(node.getStyleClass());

        Text t = new Text(2, 12, node.toString());
        t.setStyle("-fx-font-weight: bold");
        nodePanel.getChildren().add(t);

        Circle center = new Circle();
        center.setStroke(Color.RED); 
        center.setRadius(3.0);
        nodePanel.getChildren().add(center);

        Text t2 = new Text(2, 30, String.format("%s/%s - %s", xCoordinate, yCoordinate, flModifier));
        t2.setStyle("-fx-font-size: 8pt");
        nodePanel.getChildren().add(t2);
    }
}