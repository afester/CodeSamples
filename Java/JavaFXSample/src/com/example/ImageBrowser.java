package com.example;

import java.io.File;
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;

/**
 *
 * @author Dess
 */
public class ImageBrowser extends Application {
    Image[] images = new Image[100];
    ImageView[] imagesView = new ImageView[100];

@Override
public void start(Stage primaryStage) {


    StackPane root = new StackPane();

    Scene scene = new Scene(root, 600, 450);

    int j = 0;

    String path = "C://Users/afester/Documents/Andreas/Bilder/";
    String files;
    File folder = new File(path);
    File[] listOfFiles = folder.listFiles();

    for (int i = 0; i < listOfFiles.length; i++) {
        if (listOfFiles[i].getName().endsWith("jpg") || listOfFiles[i].getName().endsWith("JPG")) {
            System.out.println(listOfFiles[i].getAbsolutePath());

            files = "file:" + listOfFiles[i].getAbsolutePath();
            System.out.println(files);

            images[j] = new Image(files, 200, 200, true, true);
            imagesView[j] = new ImageView();
            imagesView[j].setImage(images[j]);
            j++;

        }
    }

    primaryStage.setTitle("Przegladarka Obrazkow");
    primaryStage.setScene(scene);
    for(int i = 0; i < imagesView.length; i++){
    root.getChildren().add(imagesView[i]);
    }
    primaryStage.show();

}

public static void main(String[] args) {
    launch(args);
}

}