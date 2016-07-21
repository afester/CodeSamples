package com.example.tree;

import javafx.scene.layout.Pane;
import javafx.scene.shape.Line;

class LayoutDataImpl<T> implements LayoutData<T> {
    int yCoordinate;
    float xCoordinate;
    float flPrelim;
    float flModifier; // mod

    TreeNode<T> prev; // left neighbor
    
    Pane nodePanel;     // reference of the JavaFX scene node which renders this tree node
    Line edgeLine;
}
