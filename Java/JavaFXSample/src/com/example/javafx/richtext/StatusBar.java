package com.example.javafx.richtext;

import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;
import javafx.scene.layout.HBox;
import javafx.scene.text.Text;

public class StatusBar extends HBox {

/*
    private StringProperty text = new SimpleStringProperty("");
    public String getText() { return text.getValue(); }
    public void setText(String txt) { text.setValue(txt); }

    
    private Text textNode = new Text();
*/
    public StatusBar() {
        setStyle("-fx-border-width: 2 0 0 2, 0 2 2 0; -fx-border-style: solid; -fx-border-color: gray, white");
        //getChildren().add(textNode);
        //textNode.textProperty().bind(text);
    }
    
}
