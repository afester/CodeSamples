package com.example.javafx.components;

import java.util.List;

import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Control;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.TreeItem;
import javafx.scene.control.TreeView;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Region;
import javafx.scene.shape.Shape;
import javafx.stage.Stage;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;


class SceneGraphItem extends TreeItem<String> {
   private Node itemNode; 
   
   public SceneGraphItem(Node node) {
      super(node.getClass().getName());
      itemNode = node;
   }
   
   public Node getNode() {
      return itemNode;
   }
}


public class SceneGraphTree {

   private Stage stage;
   private HBox layout;
   
   private ObservableList<TableData> nodeProperties;
   private TableView<TableData> table; 
   
   public SceneGraphTree() {
      stage = new Stage();
      layout = new HBox();
      Scene secondScene = new Scene(layout, 800, 600);
      stage.setScene(secondScene);
   }


   
   public static void dumpLayoutConstraints(Control node) {
      String layoutConstraints = 
            String.format("Max: %fx%f, Min: %fx%f, Actual: %fx%f", 
                          node.getMaxWidth(), node.getMaxHeight(),
                          node.getMinWidth(), node.getMinHeight(),
                          node.getWidth(), node.getHeight());
      System.err.println(layoutConstraints);
   }


   public static void dumpLayoutConstraints(Region node) {
      String layoutConstraints = 
            String.format("Max: %fx%f, Min: %fx%f, Actual: %fx%f", 
                          node.getMaxWidth(), node.getMaxHeight(),
                          node.getMinWidth(), node.getMinHeight(),
                          node.getWidth(), node.getHeight());
      System.err.println(layoutConstraints);
   }

   
   /**
    * Recursively adds all scene graph nodes to the TreeItem structure.
    * 
    * @param item
    * @param node
    */
   private void addChildNodes(TreeItem<String> item, Node node) {
      SceneGraphItem treeItem = new SceneGraphItem(node);
      item.getChildren().add(treeItem);

      if (node instanceof Parent) {
         List<Node> children = ((Parent)node).getChildrenUnmodifiable();
         for (Node child : children) {
            addChildNodes(treeItem, child);
         }
      }
   }

   
   private String oldStyle = null;

   /**
    * Sets the scene to visualize in this tree.
    *
    * @param theScene The scene to visualize.
    */
   @SuppressWarnings("unchecked")
   public void setSceneTree(Scene theScene) {
      // create a TreeItem structure with the scene graph
      Parent parentNode = theScene.getRoot();
      SceneGraphItem rootItem = new SceneGraphItem(parentNode);
      addChildNodes(rootItem, parentNode);
      rootItem.setExpanded(true);

      // create a TreeView for the TreeItems
      TreeView<String> treeView = new TreeView<String>(rootItem);
      treeView.getSelectionModel().selectedItemProperty().addListener(new ChangeListener<TreeItem<String>>() {

         @Override
         public void changed(ObservableValue<? extends TreeItem<String>> arg0,
                             TreeItem<String> oldValue, TreeItem<String> newValue) {
            SceneGraphItem newItem = (SceneGraphItem) newValue; // ?????

            if (oldStyle != null) { 
                SceneGraphItem oldItem = (SceneGraphItem) oldValue; // ?????
                Node oldNode = oldItem.getNode();
                oldNode.setStyle(oldStyle);
            }

            Node node = newItem.getNode();
            oldStyle = node.getStyle();
            node.setStyle("-fx-opacity: 0.5;");
            node.setStyle("-fx-effect: innershadow(one-pass-box, #ff0000, 10, 0.5, 1, 1);");

            // Note: need to use the ObservableList here, not its backing list!
            nodeProperties.clear();

            nodeProperties.add(new TableData("Id", node.getId()));
            nodeProperties.add(new TableData("Java class", node.getClass().getName()));
            nodeProperties.add(new TableData("Properties", "" + node.getProperties()));
            nodeProperties.add(new TableData("Bounds in local", "" + node.getBoundsInLocal()));
            nodeProperties.add(new TableData("Bounds in parent", "" + node.getBoundsInParent()));
            if (node instanceof Shape) {
                Shape s = (Shape) node;
                nodeProperties.add(new TableData("Fill", "" + s.getFill()));
                nodeProperties.add(new TableData("Stroke", "" + s.getStroke()));
            }
         }

      } );
      layout.getChildren().add(treeView); // , 0, 0);

      table = new TableView<>();

      // create an ObservableList which is backed by an ArrayList 
      // http://docs.oracle.com/javafx/2/collections/jfxpub-collections.htm
      nodeProperties = FXCollections.observableArrayList();
      table.setItems(nodeProperties);

      // set up the table columns
      TableColumn<TableData, String> nameCol = new TableColumn<>("Name");
      nameCol.setCellValueFactory(new PropertyValueFactory<TableData, String>("name"));
      nameCol.setMinWidth(120);

      TableColumn<TableData, String> valueCol = new TableColumn<>("Value");
      valueCol.setCellValueFactory(new PropertyValueFactory<TableData, String>("value"));
      valueCol.setMinWidth(500);

      table.getColumns().setAll(nameCol, valueCol);

      layout.getChildren().add(table); // , 1, 0);
   }


   public void show() {
      stage.show();
   }

}
