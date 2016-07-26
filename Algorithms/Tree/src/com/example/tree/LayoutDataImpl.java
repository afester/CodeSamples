package com.example.tree;

import javafx.scene.layout.Pane;
import javafx.scene.shape.Line;

class LayoutDataImpl<T> implements LayoutData<T> {
    float xCoordinate;  // final X coordinate of the node
    float yCoordinate;  // final Y coordinate of the node

    float flPrelim;     // preliminary X coordinates
    float flModifier;   // mod

    private TreeNode<T> leftNeighbor; // left neighbor

    Pane nodePanel;     // reference of the JavaFX scene node which renders this tree node
    Line edgeLine;
    

    /**
     * @return The preferred width of this node.
     */
    double getWidth() {
        return nodePanel.prefWidth(-1);
    }

    public double getHeight() {
        return nodePanel.prefHeight(-1);
    }

    @Override
    public String toString() {
        return String.format("LayoutDataImpl[flPrelim = %s, flModifier = %s, x=%s, y=%s]", flPrelim, flModifier, xCoordinate, yCoordinate);
    }

    public TreeNode<T> getLeftNeighbor() {
        return leftNeighbor;
    }

    public void setLeftNeighbor(TreeNode<T> theLeftNeighbor) {
        leftNeighbor = theLeftNeighbor;
    }

}
