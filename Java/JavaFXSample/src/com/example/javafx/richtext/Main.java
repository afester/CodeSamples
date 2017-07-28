package com.example.javafx.richtext;

import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

import javafx.application.Application;
import javafx.event.Event;
import javafx.geometry.Orientation;
import javafx.scene.Scene;
import javafx.scene.control.SplitPane;
import javafx.scene.control.Tab;
import javafx.scene.control.TabPane;
import javafx.scene.control.TextArea;
import javafx.scene.control.TreeItem;
import javafx.scene.control.TreeView;
import javafx.scene.layout.Priority;
import javafx.scene.layout.VBox;
import javafx.scene.text.Font;
import javafx.scene.text.Text;
import javafx.stage.Stage;

import org.fxmisc.richtext.EditableStyledDocument;
import org.fxmisc.richtext.Paragraph;
import org.fxmisc.richtext.StyleClassedTextArea;
import org.fxmisc.richtext.StyledDocument;
import org.fxmisc.richtext.StyledText;

public class Main extends Application {

    private final StyleClassedTextArea area = new StyleClassedTextArea();

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
            
            structureView.clear();
            StyledDocument<Collection<String>, Collection<String>> doc = area.getDocument();
            for (Paragraph<Collection<String>, Collection<String>> p : doc.getParagraphs()) {
                structureView.appendText(p.toString() + "\n");
                for (StyledText<Collection<String>> t : p.getSegments()) {
                    structureView.appendText("   " + t.toString() + "\n");
                }
            }

//            StyledDocument<TextStyle, ParStyle> document = area.getDocument();
//            for (Paragraph p : document.getParagraphs()) {
//                structureView.appendText("   Paragraph: " + p + "\n");
//            }
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
        tab.setOnSelectionChanged(e -> { System.err.println("EDIT"); area.requestFocus(); } );

        StatusBar statusBar = new StatusBar();
        Text paraStatus = new Text("0");
        Text posStatus = new Text("0");
        Text columnStatus = new Text("0");
        Text charactersStatus = new Text("0");
        statusBar.getChildren().addAll(new Text("Paragraph: "), paraStatus);
        statusBar.getChildren().addAll(new Text("   Abs Pos: "), posStatus);
        statusBar.getChildren().addAll(new Text("   Column: "), columnStatus);
        statusBar.getChildren().addAll(new Text("   Characters: "), charactersStatus);
        VBox editArea = new VBox();
        editArea.getChildren().addAll(area, statusBar);
        VBox.setVgrow(area, Priority.ALWAYS);
        area.currentParagraphProperty().addListener((obs, oldVal, newVal) -> paraStatus.setText(newVal.toString()) );
        area.caretPositionProperty().addListener((obs, oldVal, newVal) -> posStatus.setText(newVal.toString()) );
        area.caretColumnProperty().addListener((obs, oldVal, newVal) -> columnStatus.setText(newVal.toString()) );
        area.lengthProperty().addListener((obs, oldVal, newVal) -> charactersStatus.setText(newVal.toString()) );
        tab.setContent(editArea);

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
        mainPane.setDividerPosition(0, 0.3);

        List<String> h1 = new ArrayList<>();
        h1.add("h1");

        List<String> h2 = new ArrayList<>();
        h2.add("h2");

        List<String> h3 = new ArrayList<>();
        h3.add("h3");

        List<String> p = new ArrayList<>();
        p.add("p");

        List<String> java = new ArrayList<>();
        java.add("java");

        List<String> bold = new ArrayList<>();
        bold.add("em");

        List<String> keyword = new ArrayList<>();
        keyword.add("keyword");
        keyword.add("em");

        area.setWrapText(true);
        area.replaceText("");

        // Create StyledDocument
        List<String> PS = new ArrayList<>();
        PS.add("p");
        List<String> S = new ArrayList<>();

        EditableStyledDocument<Collection<String>, Collection<String>> doc = 
                new EditableStyledDocument<>(S, PS, true);

        StyledText<Collection<String>> seg0 = new StyledText<>("Title 1", S);
        Paragraph<Collection<String>, Collection<String>> par0 = 
                new Paragraph<>(h1, seg0);
        doc.addParagraph(par0);

        StyledText<Collection<String>> seg1 = new StyledText<>("Hello ", S);
        StyledText<Collection<String>> seg2 = new StyledText<>("World", bold);
        Paragraph<Collection<String>, Collection<String>> par1 = 
                new Paragraph<>(PS, seg1, seg2);
        doc.addParagraph(par1);

        StyledText<Collection<String>> seg3 = new StyledText<>("public class Test {\n   public static void ", S);
        StyledText<Collection<String>> seg4 = new StyledText<>("main", keyword);
        StyledText<Collection<String>> seg5 = new StyledText<>("(String[] args) {\n      System.err.println(\"Hello World\");\n   }\n}", S);
        Paragraph<Collection<String>, Collection<String>> par2 = 
                new Paragraph<>(java, seg3, seg4, seg5);
        doc.addParagraph(par2);

        area.setDocument(doc);
//        List<String> PS = new ArrayList<>();
//        List<String> S = new ArrayList<>();
//        List<String> S1 = new ArrayList<>();
//        S1.add("bold");
//        ReadOnlyStyledDocument<Collection<String>, Collection<String>> document =
//                ReadOnlyStyledDocument.fromString("Hello",  PS, S);
//        area.append(document);
//        ReadOnlyStyledDocument<Collection<String>, Collection<String>> document2 = ReadOnlyStyledDocument.fromString("World",  PS, S1);
//        area.append(document2);

/*        List<Paragraph<String, String>> paragraphs = new ArrayList<>();
        StyledDocument document = ReadOnlyStyledDocument.fromString("", "", "");
        document.concat(latter);
        document.getParagraphs();
        area.append(document);*/
/*
        DocbookImporter di = new DocbookImporter("C:\\Users\\afester\\Projects\\CodeSamples\\Python\\PyQt5\\MynPad\\S", "Sample%20Page.xml");
        di.importDocument(new DocbookHandler() {

            @Override
            public void addTitle(int level, String title) {
                area.appendText(title);
                area.setParagraphStyle(area.currentParagraphProperty().getValue(), h1);
                area.appendText("\n");
            }

            @Override
            public void addParagraph(String content) {
                area.appendText(content);
                area.setParagraphStyle(area.currentParagraphProperty().getValue(), p);
                area.appendText("\n");
            }

            @Override
            public void addCode(String content) {
                content = content.replace('\n', 'X');
                area.appendText(content);
                area.setParagraphStyle(area.currentParagraphProperty().getValue(), java);
                area.appendText("\n");
            }
            
        });
*/
/*
        area.setParagraphStyle(0, h1);

       area.appendText("\nLorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.");
       area.setParagraphStyle(1, p);

       area.appendText("\nTitle 2");
       area.setParagraphStyle(2, h2);

       area.appendText("\nLorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.");
       area.setParagraphStyle(3, p);

       area.appendText("\npublic class Test { public static void main(String[] args) {} }");
       area.setParagraphStyle(4, java);
*/
        Scene scene = new Scene(mainPane, 800, 600);
        scene.getStylesheets().add("/richtextsample.css");

        primaryStage.setScene(scene);
        primaryStage.show();
    }

}
