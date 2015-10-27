package com.example.javafx.components;

import javafx.scene.control.SkinBase;
import javafx.scene.layout.HBox;
import javafx.scene.paint.Color;


public class SevenSegmentPanelSkin extends SkinBase<SevenSegmentPanel> {

   // private SevenSegment[] digits;
    private FourteenSegment[] digits;

    // TODO: why do we need to duplicate the properties here and synchronize
    // them with the control's properties through bind()?
    // => obviously, there is no need to do this - the "LabelSkinBase" class,
    // for instance, also simply registers a listener on the Control's properties.
    // there is one detail, though: the bind() method synchronizes the 
    // properties *initially*, while the listener does not get called automatically
    // at the beginning!
    //private SimpleObjectProperty<Color> onColor = new SimpleObjectProperty<Color>();
    //private SimpleObjectProperty<Color> offColor = new SimpleObjectProperty<Color>();
    //private StringProperty text = new SimpleStringProperty();

    // TODO: actually we need a constructor with exactly one parameter (the control) 
    // (for the skin to be useable in -fx-skin CSS) - how is the code flow then? 
    // when does the UI need to be initialized? Were do we get the number of digits from?
    public SevenSegmentPanelSkin(SevenSegmentPanel control, int numberOfDigits) {
        super(control); // NOTE: SkinBase only has a Control parameter!  ... , new SevenSegmentPanelBehavior(control)));

        initialize(numberOfDigits);

        control.onColorProperty().addListener((obs, old, w) -> setOnColor(w));
        setOnColor(control.getOnColor());
        control.offColorProperty().addListener((obs, old, w) -> setOffColor(w));
        setOffColor(control.getOffColor());
        control.textProperty().addListener((obs, old, w) -> setText(w));
        setText(control.getText());

        //onColor.bind(control.onColorProperty());
        //offColor.bind(control.offColorProperty());
        //text.bind(control.textProperty());
    }

    private void initialize(int numberOfDigits) {
        final HBox displayGroup = new HBox();
        displayGroup.setId("segments");

        //digits = new SevenSegment[numberOfDigits];
        digits = new FourteenSegment[numberOfDigits];
        for (int i = 0;  i < numberOfDigits;  i++) {
            //digits[i] = new SevenSegment();
            digits[i] = new FourteenSegment();
            digits[i].setId("seg" + i);
            digits[i].getCurrentMaskProperty().addListener(l -> System.err.println("MASK CHANGED."));
            displayGroup.getChildren().add(digits[i]);
        }
        displayGroup.setMaxSize(displayGroup.getMaxWidth(), displayGroup.getPrefHeight());

        getChildren().clear();
        getChildren().add(displayGroup); // !!!! Nodes are added to the Control's children through this getChildren() method inherited from SkinBase!
    }

    private void setOnColor(Color col) {
        //for (SevenSegment s : digits) {
        for (FourteenSegment s : digits) {
            s.setOnColor(col);
        }
    }

    private void setOffColor(Color col) {
        //for (SevenSegment s : digits) {
        for (FourteenSegment s : digits) {
            s.setOffColor(col);
        }
    }

    private void setText(String text) {

        // TODO: use Java 1.8 chars() method!

        int didx = 0;
        for (int i = 0;  i < text.length() && didx < digits.length; i++) {
            char c = text.charAt(i);
            if (c == '.') {
                digits[didx-1].setDP(true); // DP is located in previous digit!
            } else {
                digits[didx].setChar(c);
                digits[didx].setDP(false);
                didx++;
            }
        }

    }
    
    
    @Override
    protected double computeMaxWidth(double height, double topInset,
            double rightInset, double bottomInset, double leftInset) {
        return computePrefWidth(height, topInset, rightInset, bottomInset, leftInset);
    }

    @Override
    protected double computeMaxHeight(double width, double topInset,
            double rightInset, double bottomInset, double leftInset) {
        return computePrefHeight(width, topInset, rightInset, bottomInset, leftInset);
    }

}
