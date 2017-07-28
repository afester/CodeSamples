package com.example.javafx;

import javafx.application.Application;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.scene.Scene;
import javafx.scene.control.Slider;
import javafx.stage.Stage;

public class SliderExample6 extends Application {

    public static void main(String[] args) { launch(args); }
    
    
    @Override
    public void start(Stage primaryStage) {
        final SnapSlider slider = new SnapSlider(0.25, 2.0, 1.0);
        slider.setShowTickLabels(true);
        slider.setShowTickMarks(true);
        slider.setMajorTickUnit(0.25);
        slider.setMinorTickCount(0);

        slider.finalValueProperty().addListener(new ChangeListener<Number>() {

            @Override
            public void changed(ObservableValue<? extends Number> observable,
                    Number oldValue, Number newValue) {
                System.err.printf("Final value: %s\n", newValue);
            }
        });

        Scene scene = new Scene(slider, 800, 600);
        primaryStage.setScene(scene);
        primaryStage.show();
    }


/*
    private boolean isFinal = true;     // assumption: no dragging - clicked value is the final one.
                                        // variable changes to "false" once dragging starts.

    private Double finalValue = null;

    @Override
    public void start(Stage primaryStage) {
        final Slider slider = new Slider(0.25, 2.0, 1.0);
        slider.setShowTickLabels(true);
        slider.setShowTickMarks(true);
        slider.setMajorTickUnit(0.25);
        slider.setMinorTickCount(0);
        slider.setSnapToTicks(true);

        slider.valueProperty().addListener(new ChangeListener<Number>() {

            final double minCompare = slider.getMin() + Math.ulp(slider.getMin());
            final double maxCompare = slider.getMax() - Math.ulp(slider.getMax());

            @Override
            public void changed(ObservableValue<? extends Number> observable,
                    Number oldValue, Number newValue) {

                if (isFinal) {  // either dragging of knob has stopped or
                                // no dragging was done at all (direct click or 
                                // keyboard navigation)
                    perform((Double) newValue);
                    finalValue = null;
                } else {        // dragging in progress

                    double val = (double) newValue;
                    if (val > maxCompare || val < minCompare) {
                        isFinal = true; // this value will be treated as final value
                                        // once the valueChangingProperty goes to false
                        finalValue = (Double) newValue; // remember current value
                    } else {
                        isFinal = false;    // no final value anymore - slider 
                        finalValue = null;  // has been dragged to a position within 
                                            // minimum and maximum
                    }

                }
            }
        });

        slider.valueChangingProperty().addListener(new ChangeListener<Boolean>() {

            @Override
            public void changed(ObservableValue<? extends Boolean> observable,
                                Boolean oldValue, Boolean newValue) {

                if (newValue == true) { // dragging of knob started.
                    isFinal = false;    // captured values are not the final ones.

                } else {                // dragging of knob stopped.

                    if (isFinal) {      // captured value is already the final one
                                        // since it is either the minimum or the maximum value
                        perform(finalValue);
                        finalValue = null;
                    } else {
                        isFinal = true; // next captured value will be the final one
                    }
                }
            }
        });

        Scene scene = new Scene(slider, 800, 600);
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    private void perform(double value) {
        System.err.printf("FINAL: %s\n", value);
    }
 */
}
