package com.example.javafx;

import com.example.javafx.components.Meter;
import com.example.javafx.components.SevenSegmentPanel;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.Button;
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


public class TableRow {
    private int xcodePoint;
    private char xcharacter;
    private long xsegmentMap;
    
    public TableRow(int codePoint) {
        this.xcodePoint = codePoint;
        this.xcharacter = (char) codePoint; 
        xsegmentMap = 0;
    }
    
    
    public String getCodePoint() {
        return String.valueOf(xcodePoint);
    }
    
    public String getCharacter() {
        return String.valueOf(xcharacter);
    }
    
    public String getSegmentMap() {
        String res = Long.toBinaryString(xsegmentMap);
        return String.format("%16s", res).replace(' ', '0');
    }
}
