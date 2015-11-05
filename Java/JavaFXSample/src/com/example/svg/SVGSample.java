package com.example.svg;

import javafx.application.Application;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.stage.Stage;

public class SVGSample extends Application {

    public static void main(String[] args) {
        launch(args);
    }


    @Override
    public void start(Stage primaryStage) {
        primaryStage.setTitle("SVG Display sample");

        SVGLoader loader = new SVGLoader();

        Group svgImage = loader.loadSvg("Ghostscript_Tiger.svg");
        //Group svgImage = loader.loadSvg("7segment.svg");
        //Group svgImage = loader.loadSvg("meter.svg");

        //Document doc = loader.loadSvgDocument("Ghostscript_Tiger.svg");
        //Group svgImage = new Group(new ImageView(loader.getImage(doc)));

        Scene scene = new Scene(svgImage, 800, 600);

        primaryStage.setScene(scene);
        primaryStage.show();
    }
}
