package com.example.javafx;

import com.example.javafx.components.Meter;
import com.example.javafx.components.SevenSegmentPanel;

import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Slider;
import javafx.scene.image.Image;
import javafx.scene.layout.Border;
import javafx.scene.layout.BorderImage;
import javafx.scene.layout.BorderRepeat;
import javafx.scene.layout.BorderStroke;
import javafx.scene.layout.BorderStrokeStyle;
import javafx.scene.layout.BorderWidths;
import javafx.scene.layout.CornerRadii;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Pane;
import javafx.scene.layout.Region;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.transform.Scale;

public class ControlPanelSample {


    public Scene createScene() {

        SevenSegmentPanel s7Panel = new SevenSegmentPanel(4, 2);
        s7Panel.setId("current");
        s7Panel.getTransforms().add(new Scale(0.6, 0.6));
        s7Panel.setValue(0);

        // Note: the single argument BorderWidths() constructor applies the given value to all edges!
        // Note: all images are drawn on top of each other.
        //       The border widths define how much from each edge of the image is to be drawn.
        //       If the image is smaller than the required area, it is tiled/stretched according to the BorderRepeat parameters.
        //       The insets define the distance between the border image and the edge of the region
        //       The slices somehow define the piece of the image which shall be drawn (?????) and they correlate with the border widths
/*        BorderImage topBorder = 
                new BorderImage(new Image("/top.png"),         // the image 
                                new BorderWidths(6, 0, 0, 0),  // the border width (top, right, bottom, left)
                                new Insets(0, 6, 0, 6),        // the insets (distance from the region border) (top, right, bottom, left) 
                                new BorderWidths(6, 0, 0, 0),  // the "slice" (top, right, bottom, left)
                                false, 
                                BorderRepeat.REPEAT, 
                                BorderRepeat.REPEAT);

        BorderImage topRightBorder = 
                new BorderImage(new Image("/topright.png"),    // the image 
                                new BorderWidths(6, 6, 0, 0),  // the border width (top, right, bottom, left)
                                new Insets(0, 0, 0, 0),        // the insets (distance from the region border) (top, right, bottom, left) 
                                new BorderWidths(6, 6, 0, 0),  // the "slice" (top, right, bottom, left)
                                false, 
                                BorderRepeat.REPEAT, 
                                BorderRepeat.REPEAT);

        BorderImage rightBorder = 
                new BorderImage(new Image("/right.png"),       // the image 
                                new BorderWidths(0, 6, 0, 0),  // the border width (top, right, bottom, left)
                                new Insets(6, 0, 6, 0),        // the insets (distance from the region border) (top, right, bottom, left) 
                                new BorderWidths(0, 6, 0, 0),  // the "slice" (top, right, bottom, left)
                                false, 
                                BorderRepeat.REPEAT, 
                                BorderRepeat.REPEAT);

        BorderImage bottomRightBorder = 
                new BorderImage(new Image("/bottomright.png"), // the image 
                                new BorderWidths(0, 6, 6, 0),  // the border width (top, right, bottom, left)
                                new Insets(0, 0, 0, 0),        // the insets (distance from the region border) (top, right, bottom, left) 
                                new BorderWidths(0, 6, 6, 0),  // the "slice" (top, right, bottom, left)
                                false, 
                                BorderRepeat.REPEAT, 
                                BorderRepeat.REPEAT);

        BorderImage bottomBorder = 
                new BorderImage(new Image("/bottom.png"),      // the image 
                                new BorderWidths(0, 0, 6, 0),  // the border width (top, right, bottom, left)
                                new Insets(0, 6, 0, 6),        // the insets (distance from the region border) (top, right, bottom, left) 
                                new BorderWidths(0, 0, 6, 0),  // the "slice" (top, right, bottom, left)
                                false, 
                                BorderRepeat.REPEAT, 
                                BorderRepeat.REPEAT);

        BorderImage bottomLeftBorder = 
                new BorderImage(new Image("/bottomleft.png"),         // the image 
                                new BorderWidths(0, 0, 6, 6),  // the border width (top, right, bottom, left)
                                new Insets(0, 0, 0, 0),         // the insets (distance from the region border) (top, right, bottom, left) 
                                new BorderWidths(0, 0, 6, 6),  // the "slice" (top, right, bottom, left)
                                false, 
                                BorderRepeat.REPEAT, 
                                BorderRepeat.REPEAT);
//****************************
        BorderImage leftBorder = 
                new BorderImage(new Image("/left.png"),         // the image 
                                new BorderWidths(0, 0, 0, 6),  // the border width (top, right, bottom, left)
                                new Insets(0, 0, 0, 0),         // the insets (distance from the region border) (top, right, bottom, left) 
                                new BorderWidths(0, 0, 0, 6),  // the "slice" (top, right, bottom, left)
                                false,
                                BorderRepeat.STRETCH, 
                                BorderRepeat.STRETCH);

        BorderImage topLeftBorder = 
                new BorderImage(new Image("/topleft.png"),         // the image 
                                new BorderWidths(6, 0, 0, 6),  // the border width (top, right, bottom, left)
                                new Insets(0, 0, 0, 0),         // the insets (distance from the region border) (top, right, bottom, left) 
                                new BorderWidths(6, 0, 0, 6),  // the "slice" (top, right, bottom, left)
                                false, 
                                BorderRepeat.REPEAT, 
                                BorderRepeat.REPEAT);
        //Border b = new Border(topBorder, topRightBorder, rightBorder, bottomRightBorder, bottomBorder, bottomLeftBorder, leftBorder, topLeftBorder);
        Border b = new Border(leftBorder);
*/
/****************************/
/*
        BorderImage sampleBorder = 
                new BorderImage(new Image("/sample.png"),         // the image 
                                new BorderWidths(10, 10, 10, 10),  // the border width (top, right, bottom, left)
                                new Insets(5, 5, 5, 5),         // the insets (distance from the region border) (top, right, bottom, left) 
                                new BorderWidths(3, 3, 3, 3),  // the "slice" (top, right, bottom, left)
                                false,
                                BorderRepeat.SPACE, 
                                BorderRepeat.SPACE);
        Border b = new Border(sampleBorder);
*/

        Image img = new Image("/border4.png");
        BorderImage sampleBorder = 
                new BorderImage(img,         // the image 
                                new BorderWidths(10, 10, 10, 10),  // the border width (top, right, bottom, left)
                                new Insets(0, 0, 0, 0),         // the insets (distance from the region border) (top, right, bottom, left) 
                                new BorderWidths(10, 10, 10, 10),  // the "slice" (top, right, bottom, left)
                                false,
                                BorderRepeat.REPEAT, 
                                BorderRepeat.REPEAT);
        Border b = new Border(sampleBorder);

        //BorderStroke bs3 = new BorderStroke(new Color(180/255f, 180/255f, 180/255f, 1.0), BorderStrokeStyle.SOLID, CornerRadii.EMPTY, new BorderWidths(2));
        //BorderStroke bs2 = new BorderStroke(new Color(190/255f, 190/255f, 190/255f, 1.0), BorderStrokeStyle.SOLID, CornerRadii.EMPTY, new BorderWidths(4));
        //BorderStroke bs1 = new BorderStroke(new Color(200/255f, 200/255f, 200/255f, 1.0), BorderStrokeStyle.SOLID, CornerRadii.EMPTY, new BorderWidths(6));

        // NOTE: Setting the border on the SevenSegmentPanel causes the border
        // to be scaled along with the panel!!!!
        //s7Panel.setBorder(b);

        //VBox group1 = new VBox(new Group(s7Panel));
        //group1.setAlignment(Pos.TOP_CENTER);
        //group1.setBorder(b);

        Slider s2 = new Slider(0.0, 1.5, 0.0);
        Meter m = new Meter();

        s2.valueProperty().addListener((obs, oldValue, newValue) -> {
                double val = newValue.doubleValue();
                if (val > 1.0) {
                    m.setValue(1.0);
                    s7Panel.setText("Err ");
                } else {
                    m.setValue(val);
                    s7Panel.setValue(val);
                }
            } );

        VBox leftGroup = new VBox();
        leftGroup.setAlignment(Pos.TOP_CENTER);
        leftGroup.setPadding(new Insets(10, 10, 10, 10));
        leftGroup.setSpacing(10);
        leftGroup.getChildren().add(m);
        Pane g = new Pane();
        s7Panel.setLayoutX(10);        
        s7Panel.setLayoutY(10);
        g.getChildren().add(new Group(s7Panel));
        g.setBorder(b);
        leftGroup.getChildren().add(new Group(g)); // new Group(s7Panel));
        leftGroup.getChildren().add(s2);

/*************************/

        SevenSegmentPanel vPanel = new SevenSegmentPanel(4, 2);
        vPanel.setId("voltage");
        vPanel.getTransforms().add(new Scale(0.6, 0.6));
        vPanel.setValue(0);
        vPanel.setBorder(b);

        Slider vSlider = new Slider(0.0, 1.5, 0.0);
        Meter vMeter = new Meter();
        vMeter.setUnit("V");

        vSlider.valueProperty().addListener((obs, oldValue, newValue) -> {
                double val = newValue.doubleValue();
                if (val > 1.0) {
                    vMeter.setValue(1.0);
                    vPanel.setText("Err ");
                } else {
                    vMeter.setValue(val);
                    vPanel.setValue(val);
                }
            } );

        VBox rightGroup = new VBox();
        rightGroup.setAlignment(Pos.TOP_CENTER);
        rightGroup.setPadding(new Insets(10, 10, 10, 10));
        rightGroup.setSpacing(10);
        // to consider a node's physical bounds as layout bounds, wrap 
        // the node in a Group (http://stackoverflow.com/questions/17699623/layout-using-the-transformed-bounds)
        rightGroup.getChildren().add(vMeter);
        rightGroup.getChildren().add(new Group(vPanel));    
        rightGroup.getChildren().add(vSlider);

        HBox mainGroup = new HBox();
        mainGroup.getChildren().add(leftGroup);
        mainGroup.getChildren().add(rightGroup);

        Scene scene = new Scene(mainGroup, 800, 600);
        scene.getStylesheets().add("/panelexample.css");

        return scene;
    }
}
