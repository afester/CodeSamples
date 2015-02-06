package com.example.javafx.components;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

import javafx.fxml.FXMLLoader;
import javafx.scene.Group;
import javafx.scene.control.Control;
import javafx.scene.paint.Color;
import javafx.scene.shape.SVGPath;

/**
 *  
 */
public class SevenSegment extends Control {

    private SVGPath[] segments = new SVGPath[7];
    private Color onColor = new Color(0x23 / 255.0, 0x2A / 255.0, 0x47 / 255.0, 1.0);
    private Color offColor = new Color(0xC9 / 255.0, 0xD3 / 255.0, 0xBB / 255.0, 1.0);
    private Color bgColor = new Color(0xDC / 255.0, 0xE1 / 255.0, 0xD1 / 255.0, 1.0);

    private boolean[][] digits = new boolean[][] {
        //     a
        //    __
        //  f| g|  b
        //    --
        //  e|__|  c
        //     d
        //
        // a     b      c      d      e      f      g
        {true,  true,  true,  true,  true,  true,  false}, // 0 
        {false, true,  true,  false, false, false, false}, // 1 
        {true,  true,  false, true,  true,  false, true},  // 2
        {true,  true,  true,  true,  false, false, true},  // 3
        {false, true,  true,  false, false, true,  true},  // 4
        {true,  false, true,  true,  false, true,  true},  // 5
        {true,  false, true,  true,  true,  true,  true},  // 6
        {true,  true,  true,  false, false, false, false}, // 7
        {true,  true,  true,  true,  true,  true,  true},  // 8
        {true,  true,  true,  true,  false, true,  true},  // 9

        {true,  true,  true,  false, true,  true,  true},  // a
        {false, false, true,  true,  true,  true,  true},  // b
        {true,  false, false, true,  true,  true,  false}, // c
        {false, true,  true,  true,  true,  false, true},  // d
        {true,  false, false, true,  true,  true,  true},  // e
        {true,  false, false, false, true,  true,  true}   // f
    };


    public SevenSegment() {

        FXMLLoader loader = new FXMLLoader();
        try {
            Group g = loader.load(new FileInputStream("7segment.fxml"));
            getChildren().add(g);

            segments[0] = (SVGPath) g.lookup("#segA");
            segments[1] = (SVGPath) g.lookup("#segB");
            segments[2] = (SVGPath) g.lookup("#segC");
            segments[3] = (SVGPath) g.lookup("#segD");
            segments[4] = (SVGPath) g.lookup("#segE");
            segments[5] = (SVGPath) g.lookup("#segF");
            segments[6] = (SVGPath) g.lookup("#segG");
            SVGPath panel = (SVGPath) g.lookup("#panel");
            panel.setFill(bgColor);

            // minimum size required for proper layouting
            setMinSize(50, 75);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }


    /**
     * Enables the necessary digits to show a (hexadecimal) number from 
     * 0 to f (15d).
     * 
     * @param number The number to show (0 to 15)
     */
    public void setDigit(int number) {
        int s = 0;
        for (SVGPath seg : segments) {
            if (digits[number][s]) {
                seg.setFill(onColor);    
            } else {
                seg.setFill(offColor);
            }
            s++;
        }
    }
}
