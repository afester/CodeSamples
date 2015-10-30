package com.example.javafx;

import com.example.javafx.components.FourteenSegment;
import com.example.javafx.components.Meter;
import com.example.javafx.components.SevenSegmentPanel;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableView;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.scene.image.Image;
import javafx.scene.layout.Border;
import javafx.scene.layout.BorderImage;
import javafx.scene.layout.BorderRepeat;
import javafx.scene.layout.BorderStroke;
import javafx.scene.layout.BorderStrokeStyle;
import javafx.scene.layout.BorderWidths;
import javafx.scene.layout.CornerRadii;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Pane;
import javafx.scene.layout.Region;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.transform.Scale;


public class SegmentEditor {


    public Scene createScene() {

        VBox leftBox = new VBox();
        leftBox.setAlignment(Pos.CENTER);

        // create panel digit
        Label label = new Label("00000000000000");

        SevenSegmentPanel s7Panel = new SevenSegmentPanel(1);
        s7Panel.setId("current");
        s7Panel.setText("");
        s7Panel.setOnDigitChanged(e -> {
            FourteenSegment digit = (FourteenSegment) e.getSource();
            String res = Long.toBinaryString(digit.getCurrentMask());
            res = String.format("%16s", res).replace(' ', '0');
            label.setText(res);
        });
        s7Panel.setOnKeyPressed(null);

        leftBox.getChildren().addAll(s7Panel, label);

        // create table
        TableView<TableRow> table = new TableView<>();
        TableColumn codepoint = new TableColumn("CodePoint");
        codepoint.setCellValueFactory(new PropertyValueFactory("codePoint"));
        TableColumn character = new TableColumn("Char");
        character.setCellValueFactory(new PropertyValueFactory("character"));
        TableColumn segmentMask = new TableColumn("Segment Mask");
        segmentMask.setCellValueFactory(new PropertyValueFactory("segmentMap"));

        table.getColumns().addAll(codepoint, character, segmentMask);

        ObservableList rows = FXCollections.observableArrayList();
        for (int i = 32;  i <= 128;  i++) {
            rows.add(new TableRow(i));
        }
        table.setItems(rows);

        // Note: A region alone does not *reposition* its children!
        // hence we wrap the LED panel in a HBox which allows us to define the
        // (unscaled) border
        HBox mainGroup = new HBox();
        mainGroup.setAlignment(Pos.CENTER);
        mainGroup.setPadding(new Insets(10, 10, 10, 10));
        mainGroup.setSpacing(10);
         mainGroup.getChildren().addAll(leftBox, table);

        Scene scene = new Scene(mainGroup, 800, 600);
        scene.getStylesheets().add("/panelexample.css");

        return scene;
    }
}
