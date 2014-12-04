package com.example.javafx.components;

import javafx.scene.CacheHint;
import javafx.scene.Group;
import javafx.scene.control.Control;
import javafx.scene.effect.Blend;
import javafx.scene.effect.BlendMode;
import javafx.scene.effect.ColorAdjust;
import javafx.scene.effect.ColorInput;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.paint.Color;

// Disclaimer: This is merely a hack to quickly get something which works :)
public class SevenSegment extends Control {

    private ImageView[] segments = new ImageView[8]; 
    private boolean[][] digits = new boolean[][] {
        //     a
        //    __
        //  f| g|  b
        //    --
        //  e|__|  c
        //     d
        //
        // a     b    c     d     e     f      g
        {true, true, true, true, true, true, false},     // 0 
        {false, true, true, false, false, false, false}, // 1 
        {true, true, false, true, true, false, true},    // 2
        {true, true, true, true, false, false, true},    // 3
        {false, true, true, false, false, true, true}, // 4
        {true, false, true, true, false, true, true}, // 5
        {true, false, true, true, true, true, true}, // 6
        {true, true, true, false, false, false, false}, // 7
        {true, true, true, true, true, true, true}, // 8
        {true, true, true, true, false, true, true}, // 9

        {true, true, true, true, true, true, true}, // a
        {true, true, true, true, true, true, true}, // b
        {true, true, true, true, true, true, true}, // c
        {true, true, true, true, true, true, true}, // d
        {true, true, true, true, true, true, true}, // e
        {true, true, true, true, true, true, true}  // f
    };
    
    public SevenSegment() {
        segments[0] = new ImageView(new Image("com/example/javafx/components/a.png"));
        segments[1] = new ImageView(new Image("com/example/javafx/components/b.png"));
        segments[2] = new ImageView(new Image("com/example/javafx/components/c.png"));
        segments[3] = new ImageView(new Image("com/example/javafx/components/d.png"));
        segments[4] = new ImageView(new Image("com/example/javafx/components/e.png"));
        segments[5] = new ImageView(new Image("com/example/javafx/components/f.png"));
        segments[6] = new ImageView(new Image("com/example/javafx/components/g.png"));
        segments[7] = new ImageView(new Image("com/example/javafx/components/dp.png"));

        ColorAdjust monochrome = new ColorAdjust();
        monochrome.setSaturation(1.0);

//        ColorAdjust blackout = new ColorAdjust();
//        blackout.setBrightness(-1.0);

        Group digit = new Group();
        for (ImageView iv : segments) {
/*
            Blend blush = new Blend(
                    BlendMode.OVERLAY,
                    monochrome,
                    new ColorInput(
                            0,
                            0,
                            segments[0].getImage().getWidth(),
                            segments[0].getImage().getHeight(),
                            Color.RED
                    )
            );

            iv.setEffect(blush);
            iv.setCache(true);
            iv.setCacheHint(CacheHint.SPEED);
*/
            digit.getChildren().add(iv);
        }

        getChildren().add(digit);
    }

    /**
     * Enables the necessary digits to show a (hexadecimal) number from 
     * 0 to f (15d).
     * 
     * @param number The number to show (0 to 15)
     */
    public void setDigit(int number) {
        int s = 0;
        for (ImageView iv : segments) {
            iv.setVisible(digits[number][s]);
            s++;
            if (s >= digits[number].length)
                break;
        }
    }
}
