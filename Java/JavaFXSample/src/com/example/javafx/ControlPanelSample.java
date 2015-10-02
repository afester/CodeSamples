package com.example.javafx;

import com.example.javafx.components.Meter;
import com.example.javafx.components.SevenSegmentPanel;

import javafx.scene.Scene;
import javafx.scene.control.Slider;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.transform.Scale;

public class ControlPanelSample {


    public Scene createScene() {

        SevenSegmentPanel s7Panel = new SevenSegmentPanel(4, 2);
        s7Panel.setId("current");
        s7Panel.getTransforms().add(new Scale(0.6, 0.6)); 
        s7Panel.setText("µA  ");

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
        leftGroup.getChildren().add(s2);
        leftGroup.getChildren().add(s7Panel);
        leftGroup.getChildren().add(m);

        SevenSegmentPanel vPanel = new SevenSegmentPanel(4, 2);
        vPanel.setId("voltage");
        vPanel.getTransforms().add(new Scale(0.6, 0.6));
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
        rightGroup.getChildren().add(vSlider);
        rightGroup.getChildren().add(vPanel);
        rightGroup.getChildren().add(vMeter);

        HBox mainGroup = new HBox();
        mainGroup.getChildren().add(leftGroup);
        mainGroup.getChildren().add(rightGroup);

        Scene scene = new Scene(mainGroup, 800, 600);
        scene.getStylesheets().add("/panelexample.css");

        return scene;
    }
}
