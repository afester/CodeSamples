package com.example.tree;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.control.ScrollPane;
import javafx.scene.layout.Pane;
import javafx.scene.layout.Region;
import javafx.scene.shape.Line;
import javafx.stage.Stage;

public class GraphicalTreeSample extends Application {


// TODO: Move to TreeSampleGenerator
    private void createSubTree(TreeNode<String> parent, int childCount, int levelCount, String label) {
       for (int cCount = 1;  cCount <= childCount;  cCount++) {
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

        TreeNode<String> tree = createTree(3, 3);

        TreeNode<String> addtlNode = tree.findNode(new TreeNode[] {
                                      new TreeNode<String>("Node.2"),
                                      new TreeNode<String>("Node.2.1"),
                                      new TreeNode<String>("Node.2.1.2") });
        addtlNode.addChildren(new TreeNode<>("Node.2.1.2.1"));
        addtlNode.addChildren(new TreeNode<>("Node.2.1.2.2"));
        addtlNode.addChildren(new TreeNode<>("Node.2.1.2.3"));

        addtlNode = tree.findNode(new TreeNode[] {
                new TreeNode<String>("Node.2"),
                new TreeNode<String>("Node.2.1"),
                new TreeNode<String>("Node.2.1.3") });
        addtlNode.addChildren(new TreeNode<>("Node.2.1.3.1"));
        addtlNode.addChildren(new TreeNode<>("Node.2.1.3.2"));
        addtlNode.addChildren(new TreeNode<>("Node.2.1.3.3"));
        
        Region mainLayout = new TreeLayout<String>(tree);

        ScrollPane s1 = new ScrollPane();
//        s1.setPrefSize(120, 120);
        s1.setContent(mainLayout);

        Scene scene = new Scene(s1, 600, 400);
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    
    public static void main(String[] args) {
        launch(args);
    }
}
