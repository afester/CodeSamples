package com.example.javafx.richtext;

import javafx.application.Application;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.Tab;
import javafx.scene.control.TabPane;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.scene.text.Text;
import javafx.scene.text.TextFlow;
import javafx.scene.web.HTMLEditor;
import javafx.stage.Stage;

public class Main extends Application {

    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage primaryStage) throws Exception {
        primaryStage.setTitle("Richtext panel sample");

        TabPane tabPane = new TabPane();

        HTMLEditor editor = new HTMLEditor();


        VBox textPanel = new VBox();
        textPanel.getStyleClass().add("richtext");

        TextFlow header = new TextFlow();
        header.getStyleClass().add("h1");

        Text headerText = new Text("Title");
        header.getChildren().add(headerText);

        Text hello = new Text("Hello");
        Text world = new Text(" World");
        world.getStyleClass().add("strong");
        world.setFill(Color.YELLOW);

        Text hello2 = new Text("Hello");
        Text moon = new Text(" Moon");
        moon.getStyleClass().add("em");

        TextFlow helloText = new TextFlow();
        helloText.getStyleClass().add("p");
        helloText.getChildren().add(hello);
        helloText.getChildren().add(world);

        TextFlow helloText2 = new TextFlow();
        helloText2.getStyleClass().add("p");
        helloText2.getChildren().add(hello2);
        helloText2.getChildren().add(moon);

        textPanel.getChildren().add(header);
        textPanel.getChildren().add(helloText);
        textPanel.getChildren().add(helloText2);

        Canvas canvas = new Canvas(300, 300);
        GraphicsContext gc = canvas.getGraphicsContext2D();
        gc.setFill(Color.BLUE);
        gc.fillText("Hello World", 20, 20);

        Tab tab = new Tab();
        tab.setText("HTMLEditor");
        tab.setContent(editor);
        tabPane.getTabs().add(tab);

        Tab tab2 = new Tab();
        tab2.setText("TextFlow");
        tab2.setContent(textPanel);
        tabPane.getTabs().add(tab2);

        Tab tab3 = new Tab();
        tab3.setText("Canvas");
        tab3.setContent(canvas);
        tabPane.getTabs().add(tab3);

        Scene scene = new Scene(tabPane, 800, 600);
        scene.getStylesheets().add("/richtextsample.css");

        primaryStage.setScene(scene);
        primaryStage.show();

    }

}
