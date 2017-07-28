package com.example.javafx;

import javafx.application.Application;
import javafx.collections.FXCollections;
import javafx.collections.ListChangeListener;
import javafx.collections.ObservableList;
import javafx.scene.Scene;
import javafx.scene.control.ListView;
import javafx.scene.control.Slider;
import javafx.stage.Stage;

public class ListExample extends Application {
    public static void main(String[] args) { launch(args); }

    @Override
    public void start(Stage primaryStage) {
        ObservableList<String> names = FXCollections.observableArrayList(
                "Julia", "Ian", "Sue", "Matthew", "Hannah", "Stephan", "Denise");
        ListView<String> listView = new ListView<String>(names);

        
        
        listView.getSelectionModel().getSelectedIndex();
        listView.getSelectionModel().getSelectedItem();
System.err.println(listView.getSelectionModel());
        listView.getSelectionModel().getSelectedIndices().addListener(
                new ListChangeListener() {
                    @Override
                    public void onChanged(Change c) {
                        System.err.println("1 IDX:" + listView.getSelectionModel().getSelectedIndex());
                        System.err.println("1 ITM:" + listView.getSelectionModel().getSelectedItem());
                        System.err.println();
                    }} ); 

        listView.getSelectionModel().getSelectedItems().addListener(
                new ListChangeListener() {

                    @Override
                    public void onChanged(Change c) {
                        System.err.println("2 IDX:" + listView.getSelectionModel().getSelectedIndex());
                        System.err.println("2 ITM:" + listView.getSelectionModel().getSelectedItem());
                        System.err.println();
                    }} ); 
        
        Scene scene = new Scene(listView, 800, 600);
        primaryStage.setScene(scene);
        primaryStage.show();
    }
    
    
}
