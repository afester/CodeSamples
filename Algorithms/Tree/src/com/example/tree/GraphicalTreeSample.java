package com.example.tree;

import javafx.application.Application;
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

        Scene scene = new Scene(s1, 600, 400);
        scene.getStylesheets().add("/com/example/tree/treelayout.css");
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    private Node createNode(TreeNode<String> node) {
        Pane result = new VBox();
        result.getStyleClass().add("TreeNode");
        result.getStyleClass().add(node.getStyleClass());

        Text t = new Text(2, 12, node.toString());
        t.setStyle("-fx-font-weight: bold");
        result.getChildren().add(t);

        
        Line hline = new Line(0, 0, 40, 0);
        result.getChildren().add(hline);

        Circle center = new Circle();
        center.setStroke(Color.RED);
        center.setRadius(3.0);
        result.getChildren().add(center);

        Circle theNode = new Circle();
        theNode.setFill(Color.YELLOW);
        theNode.setRadius(result.prefWidth(-1) / 2);
        result.getChildren().add(theNode);

        Text t2 = new Text(2, 30, String.format("%s/%s - %s", 0, 0, 0)); // xCoordinate,
                                                                         // yCoordinate,
                                                                         // flModifier));
        t2.setStyle("-fx-font-size: 8pt");
        result.getChildren().add(t2);

        return result;
    }

    private Node createEdge(TreeNode<String> node) {
        Node result = new Line();
        result.getStyleClass().add("TreeEdge");
        return result;
    }

    public static void main(String[] args) {
        launch(args);
    }
}
