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

        Region r = new Button("Hello World");
        r.setId("hello");

        SevenSegmentPanel s7Panel = new SevenSegmentPanel(10, 0);
        s7Panel.setId("current");
        s7Panel.getTransforms().add(new Scale(0.6, 0.6));
        // s7Panel.setValue(0);
        s7Panel.setText("Andreas");

        // Note: A region alone does not *reposition* its children!
        // hence we wrap the LED panel in a HBox which allows us to define the
        // (unscaled) border
        HBox borderBox = new HBox();
        borderBox.setId("currentBorder");
        borderBox.getChildren().add(new Group(s7Panel));

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

        // create the left column
        VBox leftGroup = new VBox();
        leftGroup.setAlignment(Pos.TOP_CENTER);
        leftGroup.setPadding(new Insets(10, 10, 10, 10));
        leftGroup.setSpacing(10);
        leftGroup.getChildren().add(m);
        leftGroup.getChildren().add(new Group(borderBox));
        leftGroup.getChildren().add(s2);
        leftGroup.getChildren().add(r);

/*************************/

        SevenSegmentPanel vPanel = new SevenSegmentPanel(4, 2);
        vPanel.setId("voltage");
        vPanel.getTransforms().add(new Scale(0.6, 0.6));
        vPanel.setValue(0);
        HBox vBorderBox = new HBox();
        vBorderBox.setId("voltageBorder");
        vBorderBox.getChildren().add(new Group(vPanel));

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

        // create the right column
        VBox rightGroup = new VBox();
        rightGroup.setAlignment(Pos.TOP_CENTER);
        rightGroup.setPadding(new Insets(10, 10, 10, 10));
        rightGroup.setSpacing(10);
        // to consider a node's physical bounds as layout bounds, wrap 
        // the node in a Group (http://stackoverflow.com/questions/17699623/layout-using-the-transformed-bounds)
        rightGroup.getChildren().add(vMeter);
        rightGroup.getChildren().add(new Group(vBorderBox));    
        rightGroup.getChildren().add(vSlider);

        HBox mainGroup = new HBox();
        mainGroup.getChildren().add(leftGroup);
        mainGroup.getChildren().add(rightGroup);

        Scene scene = new Scene(mainGroup, 800, 600);
        scene.getStylesheets().add("/panelexample.css");

        return scene;
    }
}
