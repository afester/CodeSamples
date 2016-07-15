package com.example.javafx;

import javafx.geometry.Insets;
import javafx.scene.image.Image;
import javafx.scene.layout.Border;
import javafx.scene.layout.BorderImage;
import javafx.scene.layout.BorderRepeat;
import javafx.scene.layout.BorderWidths;

public class BorderFactory {

    public Border createBorder() {
        // Note: the single argument BorderWidths() constructor applies the given value to all edges!
        // Note: all images are drawn on top of each other.
        //       The border widths define how much from each edge of the image is to be drawn.
        //       If the image is smaller than the required area, it is tiled/stretched according to the BorderRepeat parameters.
        //       The insets define the distance between the border image and the edge of the region
        //       The slices somehow define the piece of the image which shall be drawn (?????) and they correlate with the border widths
/*        BorderImage topBorder = 
                new BorderImage(new Image("/top.png"),         // the image 
                                new BorderWidths(6, 0, 0, 0),  // the border width (top, right, bottom, left)
                                new Insets(0, 6, 0, 6),        // the insets (distance from the region border) (top, right, bottom, left) 
                                new BorderWidths(6, 0, 0, 0),  // the "slice" (top, right, bottom, left)
                                false, 
                                BorderRepeat.REPEAT, 
                                BorderRepeat.REPEAT);

        BorderImage topRightBorder = 
                new BorderImage(new Image("/topright.png"),    // the image 
                                new BorderWidths(6, 6, 0, 0),  // the border width (top, right, bottom, left)
                                new Insets(0, 0, 0, 0),        // the insets (distance from the region border) (top, right, bottom, left) 
                                new BorderWidths(6, 6, 0, 0),  // the "slice" (top, right, bottom, left)
                                false, 
                                BorderRepeat.REPEAT, 
                                BorderRepeat.REPEAT);

        BorderImage rightBorder = 
                new BorderImage(new Image("/right.png"),       // the image 
                                new BorderWidths(0, 6, 0, 0),  // the border width (top, right, bottom, left)
                                new Insets(6, 0, 6, 0),        // the insets (distance from the region border) (top, right, bottom, left) 
                                new BorderWidths(0, 6, 0, 0),  // the "slice" (top, right, bottom, left)
                                false, 
                                BorderRepeat.REPEAT, 
                                BorderRepeat.REPEAT);

        BorderImage bottomRightBorder = 
                new BorderImage(new Image("/bottomright.png"), // the image 
                                new BorderWidths(0, 6, 6, 0),  // the border width (top, right, bottom, left)
                                new Insets(0, 0, 0, 0),        // the insets (distance from the region border) (top, right, bottom, left) 
                                new BorderWidths(0, 6, 6, 0),  // the "slice" (top, right, bottom, left)
                                false, 
                                BorderRepeat.REPEAT, 
                                BorderRepeat.REPEAT);

        BorderImage bottomBorder = 
                new BorderImage(new Image("/bottom.png"),      // the image 
                                new BorderWidths(0, 0, 6, 0),  // the border width (top, right, bottom, left)
                                new Insets(0, 6, 0, 6),        // the insets (distance from the region border) (top, right, bottom, left) 
                                new BorderWidths(0, 0, 6, 0),  // the "slice" (top, right, bottom, left)
                                false, 
                                BorderRepeat.REPEAT, 
                                BorderRepeat.REPEAT);

        BorderImage bottomLeftBorder = 
                new BorderImage(new Image("/bottomleft.png"),         // the image 
                                new BorderWidths(0, 0, 6, 6),  // the border width (top, right, bottom, left)
                                new Insets(0, 0, 0, 0),         // the insets (distance from the region border) (top, right, bottom, left) 
                                new BorderWidths(0, 0, 6, 6),  // the "slice" (top, right, bottom, left)
                                false, 
                                BorderRepeat.REPEAT, 
                                BorderRepeat.REPEAT);
//****************************
        BorderImage leftBorder = 
                new BorderImage(new Image("/left.png"),         // the image 
                                new BorderWidths(0, 0, 0, 6),  // the border width (top, right, bottom, left)
                                new Insets(0, 0, 0, 0),         // the insets (distance from the region border) (top, right, bottom, left) 
                                new BorderWidths(0, 0, 0, 6),  // the "slice" (top, right, bottom, left)
                                false,
                                BorderRepeat.STRETCH, 
                                BorderRepeat.STRETCH);

        BorderImage topLeftBorder = 
                new BorderImage(new Image("/topleft.png"),         // the image 
                                new BorderWidths(6, 0, 0, 6),  // the border width (top, right, bottom, left)
                                new Insets(0, 0, 0, 0),         // the insets (distance from the region border) (top, right, bottom, left) 
                                new BorderWidths(6, 0, 0, 6),  // the "slice" (top, right, bottom, left)
                                false, 
                                BorderRepeat.REPEAT, 
                                BorderRepeat.REPEAT);
        //Border b = new Border(topBorder, topRightBorder, rightBorder, bottomRightBorder, bottomBorder, bottomLeftBorder, leftBorder, topLeftBorder);
        Border b = new Border(leftBorder);
*/
/****************************/
/*
        BorderImage sampleBorder = 
                new BorderImage(new Image("/sample.png"),         // the image 
                                new BorderWidths(10, 10, 10, 10),  // the border width (top, right, bottom, left)
                                new Insets(5, 5, 5, 5),         // the insets (distance from the region border) (top, right, bottom, left) 
                                new BorderWidths(3, 3, 3, 3),  // the "slice" (top, right, bottom, left)
                                false,
                                BorderRepeat.SPACE, 
                                BorderRepeat.SPACE);
        Border b = new Border(sampleBorder);
*/

        Image img = new Image("/border4.png");
        BorderImage sampleBorder = 
                new BorderImage(img,         // the image 
                                new BorderWidths(10, 10, 10, 10),  // the border width (top, right, bottom, left)
                                new Insets(0, 0, 0, 0),         // the insets (distance from the region border) (top, right, bottom, left) 
                                new BorderWidths(10, 10, 10, 10),  // the "slice" (top, right, bottom, left)
                                false,
                                BorderRepeat.REPEAT, 
                                BorderRepeat.REPEAT);

        //BorderStroke bs3 = new BorderStroke(new Color(180/255f, 180/255f, 180/255f, 1.0), BorderStrokeStyle.SOLID, CornerRadii.EMPTY, new BorderWidths(2));
        //BorderStroke bs2 = new BorderStroke(new Color(190/255f, 190/255f, 190/255f, 1.0), BorderStrokeStyle.SOLID, CornerRadii.EMPTY, new BorderWidths(4));
        //BorderStroke bs1 = new BorderStroke(new Color(200/255f, 200/255f, 200/255f, 1.0), BorderStrokeStyle.SOLID, CornerRadii.EMPTY, new BorderWidths(6));

        // NOTE: Setting the border on the SevenSegmentPanel causes the border
        // to be scaled along with the panel!!!!
        //s7Panel.setBorder(b);

        return new Border(sampleBorder);
    }
}
