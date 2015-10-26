package com.example.javafx.components;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Locale;

import com.sun.javafx.css.converters.ColorConverter;

import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;
import javafx.css.CssMetaData;
import javafx.css.SimpleStyleableObjectProperty;
import javafx.css.Styleable;
import javafx.css.StyleableObjectProperty;
import javafx.css.StyleableProperty;
import javafx.scene.control.Control;
import javafx.scene.paint.Color;

/**
 * A seven segment display panel which is capable of displaying fixed point
 * numbers and some limited text.
 */
public class SevenSegmentPanel extends Control {

    private String format;
    private int numberOfDigits = 0;


/** Properties **/

    /**
     * Convenience method to display text in the display panel.
     * Supported characters are:  
     * 
     * 0123456789abcdefHlLoOPrv�. and the space character.
     *
     * @param text
     */

    /** The current text */
    private StringProperty text = new SimpleStringProperty();
    public String getText() {  return text.get();  }
    public void setText(String text) { this.text.set(text); }
    public StringProperty textProperty() { return text; }

    /** Color which defines an enabled segment */
    private StyleableObjectProperty<Color> onColor = 
            new SimpleStyleableObjectProperty<Color>(StyleableProperties.ON_COLOR, 
                                                     this, "onColor", Color.YELLOW);
    public Color getOnColor() {  return onColor.get();  }
    public void setOnColor(Color col) { onColor.set(col); }
    public StyleableObjectProperty<Color> onColorProperty() { return onColor; }

    /** Color which defines a disabled segment */
    private StyleableObjectProperty<Color> offColor = 
            new SimpleStyleableObjectProperty<Color>(StyleableProperties.OFF_COLOR, 
                                                     this, "offColor", 
                                                     new Color(0xC9 / 255.0, 0xD3 / 255.0, 0xBB / 255.0, 1.0));
    public Color getOffColor() { return offColor.get(); }
    public void setOffColor(Color col) { offColor.set(col); }
    public StyleableObjectProperty<Color> offColorProperty() { return offColor; }

    
    /**
     * Creates a new seven segment display panel.
     * Initially, all segments of the display are turned off.
     * Then, the display contents can be set with the setText() and setValue() 
     * methods.
     *
     * @param numberOfDigits The number of digits for the panel.
     */
    public SevenSegmentPanel(int numberOfDigits) {
        this(numberOfDigits, 0);
    }


    /**
     * Creates a new seven segment display panel.
     * Initially, all segments of the display are turned off.
     * Then, the display contents can be set with the setText() and setValue() 
     * methods.
     *
     * @param numberOfDigits The overall number of digits
     * @param decimalCount The number of decimal places
     */
    public SevenSegmentPanel(int numberOfDigits, int decimalCount) {
        this.getStyleClass().add("seven-segment-panel");
        this.numberOfDigits = numberOfDigits;   // TODO: make a property from this
        // create the format specifier for floating point conversion
        format = String.format("%%%d.%df", numberOfDigits + 1, decimalCount);
    }


    /**
     * Sets a floting point number to display in the display panel.
     * The number is auto-formatted to fit the setup of the display regarding
     * the number of digits and the number of decimal places.
     *
     * @param value The floating point number to display.
     */
    public void setValue(double value) {
        String text = String.format(Locale.US, format, value);    // Important: use dot as decimal point!
        setText(text);
    }


/** Skinning and style sheet support */

    @Override
    protected javafx.scene.control.Skin<?> createDefaultSkin() {
        // The skin is the actual content of the control.
        // In the most simple case, the skin is made up of one top level node
        // which is connected to the control here.

        SevenSegmentPanelSkin skin =  new SevenSegmentPanelSkin(this, numberOfDigits);
        return skin;
    };

    // This private static class defines all CSS properties which are supported by this control
    private static class StyleableProperties {

        private static final CssMetaData<SevenSegmentPanel, Color> ON_COLOR = // TODO: HOLD ON - xxxConverter classes are part of com.sun!!!!
                new CssMetaData<SevenSegmentPanel, Color>("-fx-on-color", ColorConverter.getInstance(), Color.ORANGE) {

                    @Override
                    public boolean isSettable(SevenSegmentPanel styleable) {
                        boolean result = styleable.onColor == null || !styleable.onColor.isBound();
                        return result;
                    }

                    @Override
                    public StyleableProperty<Color> getStyleableProperty(
                            SevenSegmentPanel styleable) {
                        return styleable.onColorProperty();
                    }
        };

        private static final CssMetaData<SevenSegmentPanel, Color> OFF_COLOR = // TODO: HOLD ON - xxxConverter classes are part of com.sun!!!!
                new CssMetaData<SevenSegmentPanel, Color>("-fx-off-color", ColorConverter.getInstance(), Color.BLUE) {

                    @Override
                    public boolean isSettable(SevenSegmentPanel styleable) {
                        boolean result = styleable.offColor == null || !styleable.onColor.isBound();
                        return result;
                    }

                    @Override
                    public StyleableProperty<Color> getStyleableProperty(
                            SevenSegmentPanel styleable) {
                        return styleable.offColorProperty();
                    }
        };

        private static final List<CssMetaData<? extends Styleable, ?>> PROPERTIES;

        static {
            // get list of default CSS properties from this control's parent class (Control)
            final List<CssMetaData<? extends Styleable, ?>> styleables = 
                    new ArrayList<>(Control.getClassCssMetaData());

            // add the additional properties from this class
            Collections.addAll(styleables, ON_COLOR, OFF_COLOR);

            PROPERTIES = Collections.unmodifiableList(styleables);
        }
    }

    @Override
    public List<CssMetaData<? extends Styleable, ?>> getControlCssMetaData() {
        return getClassCssMetaData();
    }

    public static List<CssMetaData<? extends Styleable, ?>> getClassCssMetaData() {
        List<CssMetaData<? extends Styleable, ?>> result = StyleableProperties.PROPERTIES;
        return result;
    }
    
}
