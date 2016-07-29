package com.example.tree;

import java.util.ArrayList;

import javafx.application.Application;
import javafx.geometry.Bounds;
import javafx.geometry.Insets;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ScrollPane;
import javafx.scene.control.TextField;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Pane;
import javafx.scene.layout.VBox;
import javafx.scene.shape.Line;
import javafx.scene.text.Text;
import javafx.stage.Stage;

class NodeData {
    private String label;
    private int order;

    public NodeData(String label) {
        this.label = label;
    }

    public String getLabel() {
        return label;
    }
    
    
    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + ((label == null) ? 0 : label.hashCode());
        return result;
    }


    @Override
    public boolean equals(Object obj) {
        if (this == obj)
            return true;
        if (obj == null)
            return false;
        if (getClass() != obj.getClass())
            return false;
        NodeData other = (NodeData) obj;
        if (label == null) {
            if (other.label != null)
                return false;
        } else if (!label.equals(other.label))
            return false;
        return true;
    }


    @Override
    public String toString() {
        return String.format("NodeData[label=%s, order=%s]", label, order);
    }

    public void setOrder(int i) {
       this.order = i;
    }

    public int getOrder() {
        return this.order;
     }
}

class ControlPanel extends VBox {
    private TreeNode<NodeData> currentNode;
    private final Button modify;
    private final Button remove;
    private final Button addChild;
    private final Button setAsRoot;
    private final Button postOrder;
    private final Button preOrder;

    private Text nodeLabelLabel = new Text("Current node:");
    private TextField nodeLabel = new TextField();

    private int order = 0;
    
    public ControlPanel(final TreeNode<NodeData> root, final TreeLayout<NodeData> layout) {
        GridPane formLayout = new GridPane();
        formLayout.setHgap(10);

        nodeLabel.setEditable(false);
        formLayout.add(nodeLabelLabel, 0, 0);
        formLayout.add(nodeLabel, 1, 0);

        HBox buttons = new HBox();
        buttons.setSpacing(10);
        modify = new Button("Modify");
        modify.setOnAction(e -> {
//            TreeNode<String> node = tree.findNode(new TreeNode[] { new TreeNode<String>("Node.1"),
//                    new TreeNode<String>("Node.1.1"), new TreeNode<String>("Node.1.1.2") });
//            node.setContent("Hello World, how are you?");
//            node.setStyleClass("redNode");
//            treeLayout.doLayout();
        });
        modify.setDisable(true);
        buttons.getChildren().add(modify);

        remove = new Button("Remove");
        remove.setDisable(true);
        buttons.getChildren().add(remove);
        remove.setOnAction(e -> {
            currentNode.getParent().removeChild(currentNode);
            layout.doLayout();
        } );

        addChild = new Button("Add child");
        addChild.setDisable(true);
        buttons.getChildren().add(addChild);
        addChild.setOnAction(e -> {
            TreeNode<NodeData> newNode = new TreeNode<>(new NodeData("Child of " + currentNode));
            currentNode.addChildren(newNode);
            layout.doLayout();
        } );

        setAsRoot = new Button("Set as root");
        setAsRoot.setDisable(true);
        buttons.getChildren().add(setAsRoot);
        setAsRoot.setOnAction(e -> {
            currentNode.setAsRoot();

            layout.setRoot(currentNode);
            layout.doLayout();
        } );

        
        HBox buttons2 = new HBox();
        postOrder = new Button("PostOrder");
        buttons2.getChildren().add(postOrder);
        postOrder.setOnAction(e -> {
            order = 0;
            // Post order traversal
            TreeTraversal<NodeData> tt2 = new DepthFirstTraversal<>();
            tt2.traversePostOrder(root, (node) -> {
                node.getContent().setOrder(order++);
            });

            layout.doLayout();
        } );

        preOrder = new Button("PreOrder");
        buttons2.getChildren().add(preOrder);
        preOrder.setOnAction(e -> {
            order = 0;
            // Post order traversal
            TreeTraversal<NodeData> tt2 = new DepthFirstTraversal<>();
            tt2.traversePreOrder(root, (node) -> {
                node.getContent().setOrder(order++);
            });

            layout.doLayout();
        } );

        getChildren().add(formLayout);
        getChildren().add(buttons);
        getChildren().add(buttons2);
        
        setSpacing(10); // spacing between rows

        setPadding(new Insets(10));
    }

    public void setSelectedNode(TreeNode<NodeData> node) {
        currentNode = node;
        nodeLabel.setText(node.toString()); // .getContent());
        
        //modify.setDisable(false);
        remove.setDisable(false);
        addChild.setDisable(false);
        setAsRoot.setDisable(false);
    }
}

public class GraphicalTreeSample extends Application {

    private Scene theScene;

    // TODO: Move to TreeSampleGenerator
    private void createSubTree(TreeNode<NodeData> parent, int childCount, int levelCount, String label) {
        for (int cCount = 1; cCount <= childCount; cCount++) {
            String newLabel = label + '.' + cCount;
            TreeNode<NodeData> child = new TreeNode<>(new NodeData(newLabel));
            parent.addChildren(child);
            if (levelCount > 1) {
                createSubTree(child, childCount, levelCount - 1, newLabel);
            }
        }
    }

    private TreeNode<NodeData> createTree(int childCount, int depth) {
        TreeNode<NodeData> root = new TreeNode<>(new NodeData("Root"));
        createSubTree(root, childCount, depth, "Node");
        return root;
    }


    @Override
    @SuppressWarnings("serial")
    public void start(Stage primaryStage) throws Exception {
        primaryStage.setTitle("JavaFX Tree viewer example");

        BorderPane mainLayout = new BorderPane();

        TreeNode<NodeData> tree = createTree(3, 3);

        TreeNode<NodeData> addtlNode = 
                tree.findNode(new ArrayList<TreeNode<NodeData>>() {{ 
                                add(new TreeNode<NodeData>(new NodeData("Node.2")));
                                add(new TreeNode<NodeData>(new NodeData("Node.2.1")));
                                add(new TreeNode<NodeData>(new NodeData("Node.2.1.2")));
                             }});
        addtlNode.addChildren(new TreeNode<>(new NodeData("Node.2.1.2.1")));
//        addtlNode.addChildren(new TreeNode<>("Node.2.1.2.2"));
        addtlNode.addChildren(new TreeNode<>(new NodeData("Node.2.1.2.3")));

        addtlNode = 
                tree.findNode(new ArrayList<TreeNode<NodeData>>() {{
                        add(new TreeNode<NodeData>(new NodeData("Node.2"))); 
                        add(new TreeNode<NodeData>(new NodeData("Node.2.1")));
                        add(new TreeNode<NodeData>(new NodeData("Node.2.1.3"))); 
                }});
        addtlNode.addChildren(new TreeNode<>(new NodeData("Node.2.1.3.1")));
        addtlNode.addChildren(new TreeNode<>(new NodeData("Node.2.1.3.2")));
        addtlNode.addChildren(new TreeNode<>(new NodeData("Node.2.1.3.3")));

        TreeLayout<NodeData> treeLayout = new TreeLayout<>(tree, this::createNode, this::createEdge);

        ScrollPane s1 = new ScrollPane();
        s1.setContent(treeLayout);

        ControlPanel ctrlPanel = new ControlPanel(tree, treeLayout);
        treeLayout.setOnNodeSelected(node -> ctrlPanel.setSelectedNode(node));
        mainLayout.setCenter(s1);
        mainLayout.setRight(ctrlPanel);

        theScene = new Scene(mainLayout, 800, 600);
        theScene.getStylesheets().add("/com/example/tree/treelayout.css");
        primaryStage.setScene(theScene);
        primaryStage.show();
    }

    

    private Node createNode(TreeNode<NodeData> node) {
        Pane result = new Pane();
        result.getStyleClass().add("TreeNode");
        result.getStyleClass().add(node.getStyleClass());

        Text t = new Text(node.getContent().getLabel());
        t.getStyleClass().add("NodeTitle");

        Line line = new Line();
        line.getStyleClass().add("TreeNode");

        Text line1 = new Text("Order: " + node.getContent().getOrder()); 
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

    private Node createEdge(TreeNode<NodeData> node) {
        Node result = new Line();
        result.getStyleClass().add("TreeEdge");
        return result;
    }

    public static void main(String[] args) {
        launch(args);
    }
}
