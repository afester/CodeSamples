package com.example.javafx.components;

import javafx.scene.control.Control;
import javafx.scene.layout.HBox;
import javafx.scene.paint.Color;

public class SevenSegmentPanel extends Control {

    private int decimalCount = 0;
    private SevenSegment[] digits;
    private int factor = 0;

    public SevenSegmentPanel(int numberOfDigits) {
        this(numberOfDigits, 0);
    }

    public SevenSegmentPanel(int numberOfDigits, int decimalCount) {
        this.decimalCount = decimalCount;
        final HBox displayGroup = new HBox();

        digits = new SevenSegment[numberOfDigits];
        for (int i = 0;  i < numberOfDigits;  i++) {
            digits[i] = new SevenSegment();
            displayGroup.getChildren().add(digits[i]);
        }
        getChildren().add(displayGroup);

        // for now, use a hard coded decimal point
        int decimalPointIdx = digits.length - decimalCount - 1;
        digits[decimalPointIdx].setDP(true);

        // this.setBackground(new Background(new BackgroundFill(Color.BLACK, new CornerRadii(0.0), new Insets(0.0))));

        factor = (int) Math.pow(10, decimalCount);
        setValue(2.56);
    }

    public void setValue(double value) {
        int whole = (int) value;
        int frac = (int) (value * factor);
        //System.err.println(whole + "/" + frac);

        int decimalPointIdx = digits.length - decimalCount - 1;
        int i = 0;
        for (i = digits.length - 1;  i > decimalPointIdx;  i--) {
           int dig = frac % 10;
           frac = frac / 10;
           //System.err.println("FRAC: " + dig + "/" + i);
           digits[i].setDigit(dig);
        }
        for (   ;  i >= 0;  i--) {
           int dig = whole % 10;
           whole = whole / 10;
           //System.err.println("WHOLE: " + dig + "/" + i);
           digits[i].setDigit(dig);
           if (whole == 0) {
               i--;
               break;
           }
        }
        for (   ;  i >= 0;  i--) {
            //System.err.println("EMPTY: " + i);
            digits[i].clear();
        }
    }

    public void setOnColor(Color color) {
        for (SevenSegment s : digits) {
            s.setOnColor(color);
        }
    }

    public void setDisplayBackground(Color color) {
        for (SevenSegment s : digits) {
            s.setDisplayBackground(color);
        }
    }

    public void setOffColor(Color color) {
        for (SevenSegment s : digits) {
            s.setOffColor(color);
        }
    }

    public void setText(String text) {
        // TODO: use Java 1.8 chars() method!

        for (int i = 0;  i < text.length() && i < digits.length; i++) {
            char c = text.charAt(i);
            digits[i].setChar(c);
        }
    }
}
