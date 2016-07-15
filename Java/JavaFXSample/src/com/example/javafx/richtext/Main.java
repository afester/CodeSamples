package com.example.javafx.richtext;

import java.util.Optional;

import org.fxmisc.richtext.InlineStyleTextArea;
import org.fxmisc.richtext.Paragraph;
import org.fxmisc.richtext.StyledDocument;
import org.fxmisc.richtext.StyledText;
import org.fxmisc.richtext.StyledTextArea;
import org.fxmisc.richtext.demo.richtext.ParStyle;
import org.fxmisc.richtext.demo.richtext.TextStyle;

import javafx.application.Application;
import javafx.event.Event;
import javafx.geometry.Orientation;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.SplitPane;
import javafx.scene.control.Tab;
import javafx.scene.control.TabPane;
import javafx.scene.control.TextArea;
import javafx.scene.control.TreeItem;
import javafx.scene.control.TreeView;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.scene.text.Text;
import javafx.scene.text.TextFlow;
import javafx.scene.web.HTMLEditor;
import javafx.stage.Stage;

public class Main extends Application {

    private final InlineStyleTextArea<TextStyle, ParStyle> area =
            new InlineStyleTextArea<>(
                    TextStyle.EMPTY.updateFontSize(12).updateFontFamily("Serif").updateTextColor(Color.BLACK),
                    TextStyle::toCss,
                    ParStyle.EMPTY,
                    ParStyle::toCss);
    {
        area.setWrapText(true);
        area.setStyleCodec(TextStyle.CODEC);
    }


    private final TextArea structureView = new TextArea();
    private final TextArea xmlView = new TextArea();
    private final TextArea htmlView = new TextArea();

    public static void main(String[] args) {
        launch(args);
    }

    
    private void activateWebView(Event e) {
        Tab tab = (Tab) e.getSource();
        if (tab.isSelected()) {
            System.err.println("Switched to Web view");    
        }
        
    }

    private void activateStructureView(Event e) {
        Tab tab = (Tab) e.getSource();
        if (tab.isSelected()) {
            structureView.setText("StyledDocument\n");
            StyledDocument<TextStyle, ParStyle> document = area.getDocument();
            for (Paragraph p : document.getParagraphs()) {
                structureView.appendText("   Paragraph: " + p + "\n");
            }
        }
    }

    private void activateXMLView(Event  e) {
        Tab tab = (Tab) e.getSource();
        if (tab.isSelected()) {
            xmlView.setText(
                    "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"+
                    "<article version=\"5.0\" xml:lang=\"en\"\n"+
                    "         xmlns=\"http://docbook.org/ns/docbook\"\n"+
                    "         xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n"+
                    "</article>");
        }
    }


    private void activateHTMLView(Event  e) {
        Tab tab = (Tab) e.getSource();
        if (tab.isSelected()) {
            htmlView.setText(
                    "<!DOCTYPE html>\n"+
                    "<html lang=\"en\">\n"+
                    "  <head></head>\n"+
                    "  <body>\n"+
                    "  </body>\n"+
                    "</html>");
        }
    }

    @Override
    public void start(Stage primaryStage) throws Exception {
        primaryStage.setTitle("Richtext panel sample");
        
        structureView.setFont(Font.font("Courier New"));
        structureView.setEditable(false);
        xmlView.setFont(Font.font("Courier New"));
        xmlView.setEditable(false);
        htmlView.setFont(Font.font("Courier New"));
        htmlView.setEditable(false);

        TabPane tabPane = new TabPane();

        Tab tab = new Tab();
        tab.setText("Edit");
        tab.setClosable(false);
        tab.setContent(area);

        Tab tab2 = new Tab();
        tab2.setText("View Web");
        tab2.setClosable(false);
        tab2.setOnSelectionChanged(e -> activateWebView(e));

        Tab tab3 = new Tab();
        tab3.setText("View PDF");
        tab3.setClosable(false);

        Tab tab4 = new Tab();
        tab4.setText("View Document Structure");
        tab4.setClosable(false);
        tab4.setContent(structureView);
        tab4.setOnSelectionChanged(e -> activateStructureView(e));

        Tab tab5 = new Tab();
        tab5.setText("View DocBook XML");
        tab5.setClosable(false);
        tab5.setContent(xmlView);
        tab5.setOnSelectionChanged(e -> activateXMLView(e));

        Tab tab6 = new Tab();
        tab6.setText("View Typo3 HTML");
        tab6.setClosable(false);
        tab6.setContent(htmlView);
        tab6.setOnSelectionChanged(e -> activateHTMLView(e));

        tabPane.getTabs().addAll(tab, tab2, tab3, tab4, tab5, tab6);

        TabPane tabPane2 = new TabPane();
        Tab tab7 = new Tab();
        tab7.setClosable(false);
        tab7.setText("Search");
        
        Tab tab8 = new Tab();
        tab8.setClosable(false);
        tab8.setText("Links from");
        
        Tab tab9 = new Tab();
        tab9.setClosable(false);
        tab9.setText("Links to");

        Tab tab10 = new Tab();
        tab10.setClosable(false);
        tab10.setText("Images");

        Tab tab11 = new Tab();
        tab11.setClosable(false);
        tab11.setText("Structure");
        tabPane2.getTabs().addAll(tab7, tab8, tab9, tab10, tab11);

        TreeItem<String> root = new TreeItem<String>("Root Node");
        root.setExpanded(true);
        root.getChildren().addAll(
            new TreeItem<String>("Item 1"),
            new TreeItem<String>("Item 2"),
            new TreeItem<String>("Item 3")
        );
        TreeView<String> treeView = new TreeView<String>(root);

        SplitPane mainPane = new SplitPane();
        SplitPane leftPane = new SplitPane();
        leftPane.setOrientation(Orientation.VERTICAL);
        leftPane.getItems().add(treeView);
        leftPane.getItems().add(tabPane2);

        mainPane.getItems().add(leftPane);
        mainPane.getItems().add(tabPane);

        
        
        DocbookImporter di = new DocbookImporter("C:\\Users\\afester\\Projects\\CodeSamples\\Python\\PyQt5\\MynPad\\S", "Sample%20Page.xml");
        di.importDocument();

//        StyledDocument<TextStyle, ParStyle> sample = new StyledDocument<>("Hello World", new TextStyle());
        // area.getDocument();
        area.appendText("Hello World");
        
        

        StyledDocument<TextStyle, ParStyle> document = area.getDocument();
        StyledText<TextStyle> t1 = new StyledText<>("Hello", new TextStyle(Optional.of(Boolean.TRUE), null, null, null, null, null, null, null));
        StyledText<TextStyle> t2 = new StyledText<>("World", new TextStyle(null, Optional.of(Boolean.TRUE), null, null, null, null, null, null));
        Paragraph<TextStyle, ParStyle> p = new Paragraph<TextStyle, ParStyle>(new ParStyle(null, null), t1, t2);

        // area.getParagraphs().add(new Paragraph<TextStyle, ParStyle>("Hello", null, null));
        // .getParagraphs().add(new Paragraph<TextStyle, ParStyle>("Hello", null, null));

        Scene scene = new Scene(mainPane, 800, 600);
        scene.getStylesheets().add("/richtextsample.css");

        primaryStage.setScene(scene);
        primaryStage.show();
    }

}
