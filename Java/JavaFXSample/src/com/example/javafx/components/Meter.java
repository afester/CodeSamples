package com.example.javafx.components;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javafx.fxml.FXMLLoader;
import javafx.scene.Group;
import javafx.scene.control.Control;
import javafx.scene.shape.SVGPath;
import javafx.scene.text.Text;
import javafx.scene.transform.Rotate;


/**
 *  
 */
public class Meter extends Control {

    private SVGPath pointer = null;
    private Text unitText = null;

    public Meter() {

        FXMLLoader loader = new FXMLLoader();
        try {
            Group g = loader.load(new FileInputStream("meter.fxml"));
            getChildren().add(g);

            pointer = (SVGPath) g.lookup("#pointer");
            unitText = (Text) g.lookup("#unitText");

            // minimum size required for proper layouting
            // setMinSize(50, 75);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    
    public void setUnit(String unit) {
    	unitText.setText(unit);
    }

    public void setValue(double doubleValue) {
        // TODO: SVG path handling is unnecessarily difficult...

    	// retrieve the width and the height - required to define the pivot point
        Pattern pattern = Pattern.compile("[Mm] (\\d+\\.\\d+),(\\d+\\.\\d+) (\\d+\\.\\d+),(\\d+\\.\\d+)");
        Matcher matcher = pattern.matcher(pointer.getContent());
        matcher.find();
        double w = Double.parseDouble(matcher.group(3));
        double h = Double.parseDouble(matcher.group(4));

        double a = doubleValue * 87.9;
        Rotate r = new Rotate(a, w, h);

        pointer.getTransforms().clear();
        pointer.getTransforms().add(r);
    }

}
