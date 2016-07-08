package com.example.tree;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;

public class TreeGrapher<T> extends Application {

    private static TreeNode rootNode;    // HACK!

    double xpos = 10;
    List<Double> xpos = new ArrayList<>();

    @Override
    public void start(Stage primaryStage) throws Exception {
        primaryStage.setTitle("JavaFX Tree viewer example");
        

        Pane mainLayout = new Pane();

        TreeTraversal<String> tt2 = new BreadthFirstTraversal<>();
        tt2.traverse(rootNode, (node) -> {
            Label label = new Label(node.toString());
            label.setLayoutX(xpos);
            label.setLayoutY(node.getLevel() * 25);
            label.setStyle("-fx-padding: 5px; -fx-background-insets: 5px; -fx-background-color: yellow; -fx-border-style: solid;");
            mainLayout.getChildren().add(label);

            xpos += 50;
        });

        
        // show the generated scene graph
        Scene scene = new Scene(mainLayout);
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    public static void main(TreeNode<String> tree) {
        rootNode = tree;    // HACK!
        launch(new String[] {});
    }
}
