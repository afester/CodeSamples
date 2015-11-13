package com.example.svg;

import com.example.javafx.tools.ColorSeparator;

import javafx.application.Application;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

public class SVGSample extends Application {

    public static void main(String[] args) {
        launch(args);
    }


    @Override
    public void start(Stage primaryStage) {
        primaryStage.setTitle("SVG Display sample");

        SVGLoader loader = new SVGLoader();

        Group svgImage = loader.loadSvg("svgSample.svg");
        //Group svgImage = loader.loadSvg("Ghostscript_Tiger.svg");
        //Group svgImage = loader.loadSvg("7segment.svg");
        //Group svgImage = loader.loadSvg("meter.svg");

        //Document doc = loader.loadSvgDocument("Ghostscript_Tiger.svg");
        //Group svgImage = new Group(new ImageView(loader.getImage(doc)));

        VBox g = new VBox();
        g.getChildren().add(svgImage);
        
        ColorSeparator cs = new ColorSeparator(loader.snapshotImage);
        Image red = cs.getRedChannel();
        Image green = cs.getGreenChannel();
        Image blue = cs.getBlueChannel();
        Image alpha = cs.getAlphaChannel();

        g.getChildren().add(new ImageView(loader.snapshotImage));
        g.getChildren().add(new ImageView(red));
        g.getChildren().add(new ImageView(green));
        g.getChildren().add(new ImageView(blue));
        g.getChildren().add(new ImageView(alpha));
        Scene scene = new Scene(g, 800, 600);

        primaryStage.setScene(scene);
        primaryStage.show();
    }
}
