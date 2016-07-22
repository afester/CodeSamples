package com.example.tree;

import javax.xml.transform.dom.DOMResult;

import javafx.application.Application;
import javafx.geometry.Bounds;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ScrollPane;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Pane;
import javafx.scene.layout.Region;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Line;
import javafx.scene.text.Text;
import javafx.stage.Stage;

public class GraphicalTreeSample extends Application {

    private Scene theScene;

    // TODO: Move to TreeSampleGenerator
    private void createSubTree(TreeNode<String> parent, int childCount, int levelCount, String label) {
        for (int cCount = 1; cCount <= childCount; cCount++) {
            String newLabel = label + '.' + cCount;
            TreeNode<String> child = new TreeNode<>(newLabel);
            parent.addChildren(child);
            if (levelCount > 1) {
                createSubTree(child, childCount, levelCount - 1, newLabel);
            }
        }
    }

    private TreeNode<String> createTree(int childCount, int depth) {
        TreeNode<String> root = new TreeNode<>("Root");
        createSubTree(root, childCount, depth, "Node");
        return root;
    }

    @Override
    public void start(Stage primaryStage) throws Exception {
        primaryStage.setTitle("JavaFX Tree viewer example");

        BorderPane mainLayout = new BorderPane();

        TreeNode<String> tree = createTree(3, 3);

        TreeNode<String> addtlNode = tree.findNode(new TreeNode[] { new TreeNode<String>("Node.2"),
                new TreeNode<String>("Node.2.1"), new TreeNode<String>("Node.2.1.2") });
        addtlNode.addChildren(new TreeNode<>("Node.2.1.2.1"));
        addtlNode.addChildren(new TreeNode<>("Node.2.1.2.2"));
        addtlNode.addChildren(new TreeNode<>("Node.2.1.2.3"));

        addtlNode = tree.findNode(new TreeNode[] { new TreeNode<String>("Node.2"), new TreeNode<String>("Node.2.1"),
                new TreeNode<String>("Node.2.1.3") });
        addtlNode.addChildren(new TreeNode<>("Node.2.1.3.1"));
        addtlNode.addChildren(new TreeNode<>("Node.2.1.3.2"));
        addtlNode.addChildren(new TreeNode<>("Node.2.1.3.3"));

        TreeLayout<String> treeLayout = new TreeLayout<>(tree, this::createNode, this::createEdge);

        HBox buttons = new HBox();
        Button modify = new Button("Modify");
        modify.setOnAction(e -> {
            TreeNode<String> node = tree.findNode(new TreeNode[] { new TreeNode<String>("Node.1"),
                    new TreeNode<String>("Node.1.1"), new TreeNode<String>("Node.1.1.2") });
            node.setContent("Hello World, how are you?");
            node.setStyleClass("redNode");
            treeLayout.doLayout();
        });
        buttons.getChildren().add(modify);

        mainLayout.setTop(buttons);
        mainLayout.setCenter(treeLayout);

        ScrollPane s1 = new ScrollPane();
        s1.setContent(mainLayout);

        theScene = new Scene(s1, 600, 400);
        theScene.getStylesheets().add("/com/example/tree/treelayout.css");
        primaryStage.setScene(theScene);
        primaryStage.show();
    }

    

    private Node createNode(TreeNode<String> node) {
        Pane result = new Pane();
        result.getStyleClass().add("TreeNode");
        result.getStyleClass().add(node.getStyleClass());

        Text t = new Text(node.toString());
        t.getStyleClass().add("NodeTitle");

        Line line = new Line();
        line.getStyleClass().add("TreeNode");

        Text line1 = new Text("# Item 1");
        line1.getStyleClass().add("NodeContent");
        Text line2 = new Text("# Item 2");
        line2.getStyleClass().add("NodeContent");

        result.getChildren().addAll(t, line, line1, line2);

        // perform a layout to get the text sizes
        Scene dummy = new Scene(result);
        dummy.getStylesheets().addAll(theScene.getStylesheets());
        result.applyCss();

        Bounds textBounds = t.getLayoutBounds();
        double width = Math.max(t.getLayoutBounds().getWidth(),
                                Math.max(line1.getLayoutBounds().getWidth(),
                                         line2.getLayoutBounds().getWidth())) + 10;
        double height = textBounds.getHeight() + 3;

        // adjust the positions of the elements
        t.setX(5);
        t.setY(t.getBaselineOffset());

        line.setStartY(height);
        line.setEndY(height);
        line.setEndX(width);

        line1.setX(2);
        line1.setY(height + line1.getBaselineOffset());
        
        line2.setX(2);
        line2.setY(line1.getY() + line2.getBaselineOffset());

        result.setPrefWidth(width);
        result.setPrefHeight(line2.getY() + 5);
        return result;
    }

    
//
//    private Node createNode1(TreeNode<String> node) {
//        Pane result = new VBox();
//        result.getStyleClass().add("TreeNode");
//        result.getStyleClass().add(node.getStyleClass());
//
//        Text t = new Text(2, 12, node.toString());
//        t.setStyle("-fx-font-weight: bold");
//        result.getChildren().add(t);
//
//        
//        Line hline = new Line(0, 0, 40, 0);
//        result.getChildren().add(hline);
//
//        Circle center = new Circle();
//        center.setStroke(Color.RED);
//        center.setRadius(3.0);
//        result.getChildren().add(center);
//
//        Circle theNode = new Circle();
//        theNode.setFill(Color.YELLOW);
//        theNode.setRadius(result.prefWidth(-1) / 2);
//        result.getChildren().add(theNode);
//
//        Text t2 = new Text(2, 30, String.format("%s/%s - %s", 0, 0, 0)); // xCoordinate,
//                                                                         // yCoordinate,
//                                                                         // flModifier));
//        t2.setStyle("-fx-font-size: 8pt");
//        result.getChildren().add(t2);
//
//        return result;
//    }

    private Node createEdge(TreeNode<String> node) {
        Node result = new Line();
        result.getStyleClass().add("TreeEdge");
        return result;
    }

    public static void main(String[] args) {
        launch(args);
    }
}
