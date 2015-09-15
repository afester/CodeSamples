package com.example.compass;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.control.Control;
import javafx.scene.control.Slider;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.text.Text;
import javafx.stage.Stage;


class Compass extends Control {

}



public class Main extends Application {

    private HBox mainLayout = new HBox();
    private VBox leftLayout = new VBox();


    @Override
    public void start(Stage stage) throws Exception {

        Slider slider = new Slider();
        slider.setMin(0);
        slider.setMax(360);

        Compass compass = new Compass();

        leftLayout.getChildren().add(slider);
        leftLayout.getChildren().add(new Text("180"));
        mainLayout.getChildren().add(leftLayout);
        mainLayout.getChildren().add(compass);

        Scene scene = new Scene(mainLayout, 800, 600);
        stage.setScene(scene);
        stage.show();
    }


    public static void main(String[] args) {
        launch(args);
    }
}
