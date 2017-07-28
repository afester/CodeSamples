package com.example.javafx;

import java.beans.XMLEncoder;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

import afester.javafx.components.MultiSegmentPanel;
import javafx.collections.FXCollections;
import javafx.collections.ListChangeListener;
import javafx.collections.ObservableList;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.control.Menu;
import javafx.scene.control.MenuBar;
import javafx.scene.control.MenuItem;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableRow;
import javafx.scene.control.TableView;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.stage.FileChooser;
import javafx.stage.Stage;


public class SegmentEditor {
    private Stage stage; 
    private ObservableList<TableRow> rows;
    private TableView<TableRow> table;

    public SegmentEditor(Stage primaryStage) {
        stage = primaryStage;
    }


    public Scene createScene() {

        VBox leftBox = new VBox();
        leftBox.setAlignment(Pos.CENTER);

        // create the menu bar
        final Menu menu1 = new Menu("File");
        final Menu menu2 = new Menu("Options");
        final Menu menu3 = new Menu("Help");

        MenuBar menuBar = new MenuBar();
        menuBar.getMenus().addAll(menu1, menu2, menu3);

        final MenuItem loadItem = new MenuItem("Load");
        loadItem.setOnAction(e -> load());
        final MenuItem saveItem = new MenuItem("Save");
        saveItem.setOnAction(e -> save());
        final MenuItem exitItem = new MenuItem("Exit");
        exitItem.setOnAction(e -> System.err.println("EXIT"));
        menu1.getItems().addAll(loadItem, saveItem, exitItem);

        // create panel digit
        Label label = new Label("00000000000000");

        MultiSegmentPanel s7Panel = new MultiSegmentPanel("7segment", 1);
        s7Panel.setId("current");
        s7Panel.setText("");

        leftBox.getChildren().addAll(s7Panel, label);

        // create table
        table = new TableView<>();
        TableColumn codepoint = new TableColumn("CodePoint");
        codepoint.setCellValueFactory(new PropertyValueFactory("codePoint"));
        TableColumn available = new TableColumn("Available?");
        available.setCellValueFactory(new PropertyValueFactory("supported"));
        TableColumn character = new TableColumn("Char");
        character.setCellValueFactory(new PropertyValueFactory("character"));
        TableColumn segmentMask = new TableColumn("Segment Mask");
        segmentMask.setCellValueFactory(new PropertyValueFactory("segmentMapString"));

        table.getColumns().addAll(codepoint, character, available, segmentMask);

        rows = FXCollections.observableArrayList();
        for (int i = 32;  i <= 128;  i++) {
            rows.add(new TableRow(i));
        }
        table.setItems(rows);
        table.getSelectionModel().getSelectedItems().addListener(new ListChangeListener<TableRow>() {

            @Override
            public void onChanged(Change c) {
                TableRow selectedRow = table.getSelectionModel().getSelectedItem();
                s7Panel.setSegmentMap(0, selectedRow.getSegmentMap());
            }
        });
        
        s7Panel.setOnDigitChanged(e -> {
            FourteenSegment digit = (FourteenSegment) e.getSource();
            String res = Long.toBinaryString(digit.getCurrentMask());
            res = String.format("%16s", res).replace(' ', '0');
            label.setText(res);
            
            TableRow selectedRow = table.getSelectionModel().getSelectedItem();
            selectedRow.setSegmentMap(digit.getCurrentMask());
            table.refresh();    // TODO: Any better approach? Actually no need to refresh the whole table here ...
        });
        
        
        
        
        // Note: A region alone does not *reposition* its children!
        // hence we wrap the LED panel in a HBox which allows us to define the
        // (unscaled) border
        HBox mainGroup = new HBox();
        mainGroup.setAlignment(Pos.CENTER);
        mainGroup.setPadding(new Insets(10, 10, 10, 10));
        mainGroup.setSpacing(10);
        mainGroup.getChildren().addAll(leftBox, table);

        BorderPane mainPanel = new BorderPane();
        mainPanel.topProperty().set(menuBar);
        mainPanel.centerProperty().set(mainGroup);

        Scene scene = new Scene(mainPanel, 800, 600);
        scene.getStylesheets().add("/panelexample.css");

        return scene;
    }

    
    public void save() {
        FileChooser fileChooser = new FileChooser();
        fileChooser.setTitle("Save digit mapping file");
        //fileChooser.getExtensionFilters().addAll(
        //        new ExtensionFilter("Text Files", "*.txt"),
        //        new ExtensionFilter("Image Files", "*.png", "*.jpg", "*.gif"),
        //        new ExtensionFilter("Audio Files", "*.wav", "*.mp3", "*.aac"),
        //        new ExtensionFilter("All Files", "*.*"));
        File selectedFile = fileChooser.showSaveDialog(stage);
        if (selectedFile != null) {
            System.err.println("Saving " + selectedFile);

            try (ObjectOutputStream dest = new ObjectOutputStream(new FileOutputStream(selectedFile))) {
                dest.writeObject(rows.toArray(new TableRow[0]));
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
/*
            try (XMLEncoder e = new XMLEncoder(
                    new BufferedOutputStream(new FileOutputStream(selectedFile)))) {
                e.writeObject(rows);
            } catch (FileNotFoundException e1) {
                e1.printStackTrace();
            }
*/

/*
            try (ObjectOutputStream dest = new ObjectOutputStream(new FileOutputStream(selectedFile))) {
                dest.writeObject(rows);
                
                
                
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
*/

        }
    }


    public void load() {
        FileChooser fileChooser = new FileChooser();
        fileChooser.setTitle("Load digit mapping file");
        //fileChooser.getExtensionFilters().addAll(
        //        new ExtensionFilter("Text Files", "*.txt"),
        //        new ExtensionFilter("Image Files", "*.png", "*.jpg", "*.gif"),
        //        new ExtensionFilter("Audio Files", "*.wav", "*.mp3", "*.aac"),
        //        new ExtensionFilter("All Files", "*.*"));
        File selectedFile = fileChooser.showOpenDialog(stage);
        if (selectedFile != null) {
            System.err.println("Loading " + selectedFile);
            
            try (ObjectInputStream source = new ObjectInputStream(new FileInputStream(selectedFile))) {
                TableRow[] loadedRows = (TableRow[]) source.readObject();
                System.err.println(loadedRows.length);
                rows = FXCollections.observableArrayList(loadedRows);
                table.setItems(rows);
                // dest.writeObject(rows.toArray());
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            } catch (ClassNotFoundException e) {
                e.printStackTrace();
            }

        }
    }
}
