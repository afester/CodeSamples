package com.example.javafx.components;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.example.svg.SVGLoader;

import javafx.fxml.FXMLLoader;
import javafx.scene.Group;
import javafx.scene.Node;
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
    private double w = 0;
    private double h = 0;

    public Meter() {
        SVGLoader loader = new SVGLoader();
        Node iv = loader.loadSvg("meter.svg");
        pointer = (SVGPath) iv.lookup("#pointer");

        // get the pivot point - isn't there any easier way??? 
        Pattern pattern = Pattern.compile("[Mm] (\\d+\\.\\d+),(\\d+\\.\\d+) (\\d+\\.\\d+),(\\d+\\.\\d+)");
        Matcher matcher = pattern.matcher(pointer.getContent());
        matcher.find();
        w = Double.parseDouble(matcher.group(3));
        h = Double.parseDouble(matcher.group(4));

        unitText = (Text) iv.lookup("#unitText");
        // System.err.println(unitText);

        getChildren().add(iv);
/*
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
        */
    }


    @Override
    protected javafx.scene.control.Skin<?> createDefaultSkin() {
        return new MeterSkin(this);
    };

    
    public void setUnit(String unit) {
    	unitText.setText(unit);
    }

    public void setValue(double doubleValue) {
        // TODO: SVG path handling is unnecessarily difficult...

    	// retrieve the width and the height - required to define the pivot point
        //Pattern pattern = Pattern.compile("[Mm] (\\d+\\.\\d+),(\\d+\\.\\d+) (\\d+\\.\\d+),(\\d+\\.\\d+)");
        //Matcher matcher = pattern.matcher(pointer.getContent());
        //matcher.find();

        //double w = Double.parseDouble(matcher.group(3));
        //double h = Double.parseDouble(matcher.group(4));
        double a = doubleValue * 87.9;
        Rotate r = new Rotate(a, w, h);

        pointer.getTransforms().clear();
        pointer.getTransforms().add(r);
    }

}
