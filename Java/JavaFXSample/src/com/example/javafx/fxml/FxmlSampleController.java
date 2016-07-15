package com.example.javafx.fxml;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.Node;
import javafx.stage.DirectoryChooser;
import javafx.stage.Window;

public class FxmlSampleController {

    private DirectoryChooser sourceDirectoryChooser = new DirectoryChooser();

    @FXML
    private void handleSourceBrowse(ActionEvent ae) {
        Node source = (Node) ae.getSource();
        Window theStage = source.getScene().getWindow();

        sourceDirectoryChooser.showDialog(theStage);
    }
}
