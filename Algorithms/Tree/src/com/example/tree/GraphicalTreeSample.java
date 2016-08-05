package com.example.tree;

import java.io.File;
import java.io.FileOutputStream;
import java.io.PrintStream;
import java.util.List;
import java.util.ListIterator;

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
import javafx.scene.shape.CubicCurve;
import javafx.scene.shape.Line;
import javafx.scene.shape.QuadCurve;
import javafx.scene.text.Text;
import javafx.stage.FileChooser;
import javafx.stage.Stage;

class NodeData {

    private String label;
    private int order;
    private String additionalText; 

    public NodeData(String label) {
        this(label, 0, "");
    }

    public NodeData(String label, int order, String additionalText) {
        this.label = label;
        this.order = order;
        this.additionalText = additionalText;
    }


    public void setLabel(String string) {
        label = string;
    }

    public String getLabel() {
        return label;
    }

    public void setOrder(int i) {
       this.order = i;
    }

    public int getOrder() {
       return this.order;
    }

    public void setAdditionalText(String value) {
        additionalText = value;
    }

    public String getAdditionalText() {
        return additionalText;
    }
    
    
    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + ((additionalText == null) ? 0 : additionalText.hashCode());
        result = prime * result + ((label == null) ? 0 : label.hashCode());
        result = prime * result + order;
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
        if (additionalText == null) {
            if (other.additionalText != null)
                return false;
        } else if (!additionalText.equals(other.additionalText))
            return false;
        if (label == null) {
            if (other.label != null)
                return false;
        } else if (!label.equals(other.label))
            return false;
        if (order != other.order)
            return false;
        return true;
    }

    @Override
    public String toString() {
        return String.format("NodeData[label=%s, order=%s, additionalText=%s]", label, order, additionalText);
    }
}

class ControlPanel extends VBox {
    private TreeNode<NodeData> currentNode;
    private TreeNode<NodeData> rootNode;
    private final Button modify;
    private final Button remove;
    private final Button addChild;
    private final Button setAsRoot;
    private final Button postOrder;
    private final Button preOrder;
    private final Button saveTree;
    private final Button loadTree;
    private final Button modifyShape;

    private Text nodeLabelLabel = new Text("Node:");
    private Text nodeLabel = new Text();
    private Text nodeTitleLabel = new Text("Title:");
    private TextField nodeTitle= new TextField();
    private Text nodeAdditionalTextLabel= new Text("Text:");
    private TextField nodeAdditionalText= new TextField();

    private int order = 0;
    
    public ControlPanel(final TreeLayout<NodeData> layout) {
//        rootNode = theRoot;

        GridPane formLayout = new GridPane();
        formLayout.setHgap(10);

        formLayout.add(nodeLabelLabel, 0, 0);
        formLayout.add(nodeLabel, 1, 0);

        formLayout.add(nodeTitleLabel, 0, 1);
        formLayout.add(nodeTitle, 1, 1);

        formLayout.add(nodeAdditionalTextLabel, 0, 2);
        formLayout.add(nodeAdditionalText, 1, 2);

        HBox buttons = new HBox();
        buttons.setSpacing(10);
        modify = new Button("Update");
        modify.setOnAction(e -> {
            NodeData nd = currentNode.getContent();
            nd.setLabel(nodeTitle.getText());
            nd.setAdditionalText(nodeAdditionalText.getText());
            layout.doLayout();
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
            rootNode = currentNode;

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
            tt2.traversePostOrder(rootNode, (node) -> {
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
            tt2.traversePreOrder(rootNode, (node) -> {
                node.getContent().setOrder(order++);
            });

            layout.doLayout();
        } );

        

        HBox buttons3 = new HBox();
        loadTree = new Button("Load");
        buttons3.getChildren().add(loadTree);
        loadTree.setOnAction(e -> {
            FileChooser fileChooser = new FileChooser();
            fileChooser.setTitle("Load tree ...");
            File selectedFile = fileChooser.showOpenDialog(null); // mainStage);
            if (selectedFile != null) {
                rootNode = TreeReader.loadTree(selectedFile);
                layout.setRoot(rootNode);
                layout.doLayout();
            }
        } );
        saveTree = new Button("Save");
        buttons3.getChildren().add(saveTree);
        saveTree.setOnAction(e -> {
            FileChooser fileChooser = new FileChooser();
            fileChooser.setTitle("Save tree ...");
            File selectedFile = fileChooser.showSaveDialog(null); // mainStage);
            if (selectedFile != null) {
                
                try {
                    TreeAsciiRenderer<NodeData> ren = new TreeAsciiRenderer<NodeData>(new PrintStream(new FileOutputStream(selectedFile)));
                    ren.renderExt(rootNode);
                } catch (Exception e1) {
                    e1.printStackTrace();
                }
            }
        } );

        modifyShape = new Button("modify shape");
        buttons3.getChildren().add(modifyShape);
        modifyShape.setOnAction(e -> {
            layout.modifyShape();
        } );

        getChildren().add(formLayout);
        getChildren().add(buttons);
        getChildren().add(buttons2);
        getChildren().add(buttons3);
        
        setSpacing(10); // spacing between rows

        setPadding(new Insets(10));
    }

    public void setSelectedNode(TreeNode<NodeData> node) {
        currentNode = node;

        List<TreeNode<NodeData>> path = node.getPath();
        StringBuilder sb = new StringBuilder();
        for (ListIterator<TreeNode<NodeData>> it = path.listIterator(path.size()); it.hasPrevious();) {
            TreeNode<NodeData> element = it.previous();
            if (sb.length() > 0) {
                sb.append(" => ");
            }
            sb.append(element.getContent().getLabel());
        }

        nodeLabel.setText(sb.toString());
        nodeTitle.setText(node.getContent().getLabel());
        nodeAdditionalText.setText(node.getContent().getAdditionalText());
        modify.setDisable(false);
        remove.setDisable(false);
        addChild.setDisable(false);
        setAsRoot.setDisable(false);
    }
}

public class GraphicalTreeSample extends Application {

    private Scene theScene;

    @Override
    public void start(Stage primaryStage) throws Exception {
        primaryStage.setTitle("JavaFX Tree viewer example");

        BorderPane mainLayout = new BorderPane();
        
        TreeNode<NodeData> rootNode = TreeReader.loadTree(new File("C:\\Users\\afester\\Projects\\CodeSamples\\Algorithms\\Tree\\Negative.tree"));
        TreeLayout<NodeData> treeLayout = new TreeLayout<>(rootNode, this::createNode, this::createEdge);

        ScrollPane s1 = new ScrollPane();
        s1.setContent(treeLayout);

        ControlPanel ctrlPanel = new ControlPanel(treeLayout);
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
        Text line2 = new Text(node.getContent().getAdditionalText());
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


    private Node createEdge(@SuppressWarnings("unused") TreeNode<NodeData> node) {
        //Node result = new Line();
        Node result = new CubicCurve();
        result.getStyleClass().add("TreeEdge");
        return result;
    }

    public static void main(String[] args) {
        launch(args);
    }
}
