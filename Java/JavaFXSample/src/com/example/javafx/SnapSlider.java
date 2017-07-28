package com.example.javafx;

import javafx.beans.property.ReadOnlyDoubleProperty;
import javafx.beans.property.ReadOnlyDoubleWrapper;
import javafx.scene.control.Slider;

/**
 * Sub class of Slider which only returns values once they are snapped to 
 * corresponding ticks.
 */
public class SnapSlider extends Slider {

    private boolean isFinal = true;     // assumption: no dragging - clicked value is the final one.
                                        // variable changes to "false" once dragging starts.
    private Double storedValue = null;

    // The finalValue property - this property is modified once the snapping 
    // slider has reached a final value 
    private final ReadOnlyDoubleWrapper finalValue = new ReadOnlyDoubleWrapper();
    public Double getFinalValue() {  return finalValue.get();  }
    public final ReadOnlyDoubleProperty finalValueProperty() { return finalValue.getReadOnlyProperty(); }

    /**
     * {@inheritDoc} 
     */
    public SnapSlider(double min, double max, double value) {
        super(min, max, value);
        setSnapToTicks(true);

        final double minCompare = min + Math.ulp(min);
        final double maxCompare = max - Math.ulp(max);

        valueProperty().addListener((observable, oldValue, newValue) -> {
            if (isFinal) {  // either dragging of knob has stopped or
                            // no dragging was done at all (direct click or 
                            // keyboard navigation)
                finalValue.set((Double) newValue);
                storedValue = null;
            } else {        // dragging in progress

                double val = (double) newValue;
                if (val > maxCompare || val < minCompare) {
                    isFinal = true; // this value will be treated as final value
                                    // once the valueChangingProperty goes to false
                    storedValue = (Double) newValue; // remember current value
                } else {
                    isFinal = false;    // no final value anymore - slider 
                    storedValue = null; // has been dragged to a position within 
                                        // minimum and maximum
                }

            }
        });

        valueChangingProperty().addListener((observable, oldValue, newValue) -> {
            if (newValue == false && isFinal == true) { // dragging of knob stopped
                                                        // AND captured value is already the final one
                                                        // since it is either the minimum or the maximum value
                finalValue.set(storedValue);
                storedValue = null;
            }

            isFinal = !newValue;        // adjust the isFinal flag according to the new property value
        });
    }
}
