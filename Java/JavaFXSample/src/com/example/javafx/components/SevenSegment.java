package com.example.javafx.components;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

import javafx.fxml.FXMLLoader;
import javafx.scene.CacheHint;
import javafx.scene.Group;
import javafx.scene.Parent;
import javafx.scene.control.Control;
import javafx.scene.effect.Blend;
import javafx.scene.effect.BlendMode;
import javafx.scene.effect.ColorAdjust;
import javafx.scene.effect.ColorInput;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.paint.Color;
import javafx.scene.shape.LineTo;
import javafx.scene.shape.MoveTo;
import javafx.scene.shape.Path;

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
/*        
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
*/
//        ColorAdjust blackout = new ColorAdjust();
//        blackout.setBrightness(-1.0);

//        Group digit = new Group();
 //       for (ImageView iv : segments) {
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
//            digit.getChildren().add(iv);
 //       }

        FXMLLoader loader = new FXMLLoader();
        try {
            loader.load(new FileInputStream("sample.fxml"));
        } catch (FileNotFoundException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }// getClass().getResource("fxml_example.fxml"));

        Path path = createPath1();
        path.setScaleX(10);
        path.setScaleY(10);
        path.setStroke(Color.RED);
        path.setStrokeWidth(0.1);
        getChildren().add(path);

        path = createPath2();
        path.setScaleX(10);
        path.setScaleY(10);
        path.setStroke(Color.YELLOW);
        path.setStrokeWidth(0.1);
        getChildren().add(path);

    }
    
    
    public Path createPath2() {
        Path path = new Path();
        MoveTo moveTo = new MoveTo();
        moveTo.setX(5.179016f);
        moveTo.setY(10.826772f);
        path.getElements().add(moveTo);

        LineTo lineTo0 = new LineTo();
        lineTo0.setX(4.269803f);
        lineTo0.setY(10.826772f);
        path.getElements().add(lineTo0);

        LineTo lineTo1 = new LineTo();
        lineTo1.setX(3.815196f);
        lineTo1.setY(10.039370f);
        path.getElements().add(lineTo1);

        LineTo lineTo2 = new LineTo();
        lineTo2.setX(4.269803f);
        lineTo2.setY(9.251969f);
        path.getElements().add(lineTo2);

        LineTo lineTo3 = new LineTo();
        lineTo3.setX(5.179016f);
        lineTo3.setY(9.251969f);
        path.getElements().add(lineTo3);

        LineTo lineTo4 = new LineTo();
        lineTo4.setX(5.633622f);
        lineTo4.setY(10.039370f);
        path.getElements().add(lineTo4);

        LineTo lineTo5 = new LineTo();
        lineTo5.setX(5.179016f);
        lineTo5.setY(10.826772f);
        path.getElements().add(lineTo5);
        
        return path;
    }
    public Path createPath1() {
        Path path = new Path();
        MoveTo moveTo = new MoveTo();
        moveTo.setX(1.871929f);
        moveTo.setY(5.511811f);
        path.getElements().add(moveTo);

        LineTo lineTo0 = new LineTo();
        lineTo0.setX(0.962716f);
        lineTo0.setY(5.511811f);
        path.getElements().add(lineTo0);

        LineTo lineTo1 = new LineTo();
        lineTo1.setX(0.508110f);
        lineTo1.setY(4.724409f);
        path.getElements().add(lineTo1);

        LineTo lineTo2 = new LineTo();
        lineTo2.setX(0.962716f);
        lineTo2.setY(3.937008f);
        path.getElements().add(lineTo2);

        LineTo lineTo3 = new LineTo();
        lineTo3.setX(1.871929f);
        lineTo3.setY(3.937008f);
        path.getElements().add(lineTo3);

        LineTo lineTo4 = new LineTo();
        lineTo4.setX(2.326536f);
        lineTo4.setY(4.724409f);
        path.getElements().add(lineTo4);

        LineTo lineTo5 = new LineTo();
        lineTo5.setX(1.871929f);
        lineTo5.setY(5.511811f);
        path.getElements().add(lineTo5);        
        
        return path;
    }

    /**
     * Enables the necessary digits to show a (hexadecimal) number from 
     * 0 to f (15d).
     * 
     * @param number The number to show (0 to 15)
     */
    public void setDigit(int number) {
        /*
        int s = 0;
        for (ImageView iv : segments) {
            iv.setVisible(digits[number][s]);
            s++;
            if (s >= digits[number].length)
                break;
        }*/
    }
}
