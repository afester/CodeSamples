package com.example.javafx.tools;

import java.nio.ByteBuffer;

import javafx.scene.Node;
import javafx.scene.SnapshotParameters;
import javafx.scene.effect.Blend;
import javafx.scene.effect.BlendMode;
import javafx.scene.effect.ColorInput;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.image.PixelFormat;
import javafx.scene.image.PixelReader;
import javafx.scene.image.PixelWriter;
import javafx.scene.image.WritableImage;
import javafx.scene.paint.Color;

public class ColorSeparator {

    private Image sourceImage;
    
    public ColorSeparator(Image image) {
        sourceImage = image;
    }

    public Image getBlueChannel() {
        
        Node img = new ImageView(sourceImage);

        //img.setBlendMode(BlendMode.BLUE);

        Blend blend = new Blend();
        //blend.setMode(BlendMode.EXCLUSION);
        //blend.setMode(BlendMode.COLOR_DODGE);
        //blend.setMode(BlendMode.DIFFERENCE);
        //blend.setMode(BlendMode.COLOR_BURN);
        blend.setMode(BlendMode.MULTIPLY);

        // set top input
        ColorInput colorInput = new ColorInput();
        colorInput.setPaint(Color.BLUE);
        colorInput.setX(0);
        colorInput.setY(0);
        colorInput.setWidth(sourceImage.getWidth());
        colorInput.setHeight(sourceImage.getHeight());
        blend.setTopInput(colorInput);

        System.err.println(blend.getTopInput());
        System.err.println(blend.getBottomInput());
        // set bottom input
        img.setEffect(blend);
        System.err.println(blend.getTopInput());
        System.err.println(blend.getBottomInput());

        SnapshotParameters params = new SnapshotParameters();
        Image result = img.snapshot(params, null);

        return result;
    }

    public Image getBlueChannel1() {
        PixelReader pr = sourceImage.getPixelReader();
        int width = (int) sourceImage.getWidth();
        int height = (int) sourceImage.getHeight();

        WritableImage result = new WritableImage(width, height);
        PixelWriter pw = result.getPixelWriter();
        for (int x = 0;  x < width;  x++) {
            for (int y = 0; y < height;  y++) {
                Color col = pr.getColor(x, y);
                pw.setColor(x, y, new Color(0, 0, col.getBlue(), 1.0));    
            }
        }

        return result;
    }

}
