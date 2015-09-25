package com.example.javafx.components;

import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.Slider;
import javafx.scene.image.Image;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.paint.ImagePattern;
import javafx.scene.transform.Scale;

public class ControlPanelSample {

    
    
    
    
    public Scene createScene() {

        SevenSegmentPanel s7Panel = new SevenSegmentPanel(4, 2);

        s7Panel.setOnColor(new Color(0.95, 0.0, 0.0, 1.0));
        s7Panel.setOffColor(new Color(0.3, 0.0, 0.0, 1.0));
        s7Panel.setDisplayBackground(new Color(0.35, 0.0, 0.0, 1.0));

        // s7Panel.getTransforms().add(new Scale(0.6, 0.6)); // -fx-on-color: #f20000; 
        s7Panel.setStyle("-fx-scale-x: 0.6; -fx-scale-y: 0.6;  -fx-box-border: ladder(    -fx-color,    black 20%,    derive(-fx-color,-15%) 30%);");

        s7Panel.setText("µA  ");

        Slider s2 = new Slider(0.0, 1.5, 0.0);
        Meter m = new Meter();

        s2.valueProperty().addListener(
                new ChangeListener<Number>() {

                    @Override
                    public void changed(ObservableValue<? extends Number> arg0,
                            Number oldValue, Number newValue) {
                        double val = newValue.doubleValue();
                        if (val > 1.0) {
                            m.setValue(1.0);
                            s7Panel.setText("Err ");
                        } else {
                            m.setValue(val);
                            s7Panel.setValue(val);
                        }
                    }
                } );

        VBox leftGroup = new VBox();
        leftGroup.getChildren().add(s2);
        leftGroup.getChildren().add(s7Panel);
        leftGroup.getChildren().add(m);

        SevenSegmentPanel vPanel = new SevenSegmentPanel(4, 2);
        vPanel.setOnColor(new Color(0.95, 0.0, 0.0, 1.0));
        vPanel.setOffColor(new Color(0.3, 0.0, 0.0, 1.0));
        vPanel.setDisplayBackground(new Color(0.35, 0.0, 0.0, 1.0));
        vPanel.getTransforms().add(new Scale(0.6, 0.6));
        Slider vSlider = new Slider(0.0, 1.5, 0.0);
        Meter vMeter = new Meter();
        vMeter.setUnit("V");

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
