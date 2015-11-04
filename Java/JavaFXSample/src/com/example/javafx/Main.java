package com.example.javafx;

import com.example.javafx.components.Meter;
import com.example.javafx.components.SevenSegmentPanel;
import com.example.svg.SVGLoader;

import javafx.animation.Interpolator;
import javafx.animation.Timeline;
import javafx.animation.TranslateTransition;
import javafx.application.Application;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Point3D;
import javafx.geometry.Pos;
import javafx.geometry.Rectangle2D;
import javafx.scene.Group;
import javafx.scene.Node;
import javafx.scene.PerspectiveCamera;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.CheckBox;
import javafx.scene.control.Slider;
import javafx.scene.image.Image;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.paint.Color;
import javafx.scene.paint.CycleMethod;
import javafx.scene.paint.ImagePattern;
import javafx.scene.paint.LinearGradient;
import javafx.scene.paint.Stop;
import javafx.scene.shape.Arc;
import javafx.scene.shape.Cylinder;
import javafx.scene.shape.Ellipse;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Text;
import javafx.scene.transform.Scale;
import javafx.scene.transform.Shear;
import javafx.stage.Screen;
import javafx.stage.Stage;
import javafx.util.Duration;

public class Main extends Application {

    private int rowCount = 0;
    private GridPane grid = new GridPane();
    private int number = 0;

    public static void main(String[] args) {
        launch(args);
    }


    private void addRow(Node ... nodes) {
        int column = 0;
        for(Node node : nodes) {
            grid.add(node, column, rowCount);
            column++;
        }
        rowCount++;
    }


    //@Override
    public void start(Stage primaryStage) {
        primaryStage.setTitle("Control panel sample");

        ControlPanelSample cps = new ControlPanelSample();
        //SegmentEditor cps = new SegmentEditor(primaryStage);
        Scene scene = cps.createScene();

        primaryStage.setScene(scene);
        primaryStage.show();
    }


    //@Override
    public void start2(Stage primaryStage) {
        primaryStage.setTitle("Sample application for JavaFX components");

        final Group mainGroup = new Group();

        grid.setAlignment(Pos.TOP_LEFT);
        // grid.setGridLinesVisible(true);

        addRow(new Text("Screens:"), new Text(Integer.toString(Screen.getScreens().size())));
        Rectangle2D screenBounds = Screen.getPrimary().getVisualBounds();
        addRow(new Text("Primary Screen:"), new Text(String.format("%f x %f", screenBounds.getWidth(), screenBounds.getHeight())));

        Text textRef = new Text("Hello World");
        addRow(new Text("Text:"), textRef);

        Button but = new Button("Ok");
        addRow(new Text("Button:"), but);

        final Group g1 = new Group();
        g1.getTransforms().add(new Shear(-0.5, 0.0));
        Text t2 = new Text("Text in Group");
        Button b2 = new Button("Button in Group");
        g1.getChildren().add(t2);
        g1.getChildren().add(b2);

        final CheckBox c1 = new CheckBox("Toggle group");
        c1.setSelected(true);
        c1.setOnAction(new EventHandler<ActionEvent>() {

            @Override
            public void handle(ActionEvent arg0) {
                g1.setVisible(c1.isSelected());
            }
        });

        Slider s1 = new Slider(0.0, 1.0, 1.0);
        s1.valueProperty().addListener(/*
                                        * (val, oldVal, newVal) -> {
                                        * g1.setOpacity(newVal.doubleValue());
                                        * });
                                        */

        new ChangeListener<Number>() {

            @Override
            public void changed(ObservableValue<? extends Number> arg0,
                    Number oldValue, Number newValue) {
                g1.setOpacity(newValue.doubleValue());
            }
        });
        addRow(new Text("Group"), g1, c1, s1);

        Stop[] stops = new Stop[] { new Stop(0, Color.web("0x4EBBCC")),
                new Stop(1, Color.web("0x6D84A3")) };
        LinearGradient linearGradient = new LinearGradient(0, 0, 0, 1, true,
                CycleMethod.NO_CYCLE, stops);

        Rectangle r1 = new Rectangle(100, 100);
        r1.setFill(linearGradient);

        Ellipse e1 = new Ellipse(30, 80);
        e1.setRotate(45);
        e1.setFill(linearGradient);

        Arc a1 = new Arc(100, 100, 30, 80, 45, 200);
        a1.setFill(linearGradient);

        // sample 3D shape
        Cylinder cyl1 = new Cylinder(50, 100);
        cyl1.setRotationAxis(new Point3D(25, 100, 20));
        cyl1.setRotate(180);

        addRow(new Text("Shapes:"), r1, e1, a1, cyl1);

        SevenSegmentPanel s7Panel = new SevenSegmentPanel(4, 2);
        s7Panel.setOnColor(new Color(0.95, 0.0, 0.0, 1.0));
        s7Panel.setOffColor(new Color(0.3, 0.0, 0.0, 1.0));
        // s7Panel.setDisplayBackground(new Color(0.35, 0.0, 0.0, 1.0));
        s7Panel.getTransforms().add(new Scale(0.6, 0.6));
        //s7Panel.setText("µA  ");
        s7Panel.setText("1.234");
        addRow(new Text("Seven segment display:"), s7Panel);

        Slider s2 = new Slider(0.0, 1.5, 0.0);
        Meter m = new Meter();

        final HBox displayGroup2 = new HBox();
        displayGroup2.getChildren().addAll(new Text("Meter (As JavaFX nodes):"), m, s2);
        addRow(displayGroup2);

        s2.valueProperty().addListener( 
            (arg0, oldValue, newValue) -> {  
                double val = newValue.doubleValue();
                if (val > 1.0) {
                    m.setValue(1.0);
                    s7Panel.setText("Err ");
                } else {
                    m.setValue(val);
                    s7Panel.setValue(val);
                }
            }  );

    //    SVGLoader loader = new SVGLoader();
    //    Node treenode = loader.loadSvg("tree.svg");
    //    addRow(new Text("Tree"), treenode);

        
/*

        ImageView iv = new ImageView();

        final HBox displayGroup2 = new HBox();
        displayGroup2.getChildren().addAll(new Text("Meter (Rendered):"), iv, s2);
        addRow(displayGroup2);

        Document doc = loader.loadSvgDocument("meter.svg");
        SVGOMPathElement e = (SVGOMPathElement) doc.getElementById("pointer");

        Pattern pattern = Pattern.compile("[Mm] (\\d+\\.\\d+),(\\d+\\.\\d+) (\\d+\\.\\d+),(\\d+\\.\\d+)");
        Matcher matcher = pattern.matcher(e.getAttribute("d"));
        matcher.find();
        double w = Double.parseDouble(matcher.group(3));
        double h = Double.parseDouble(matcher.group(4));

        SVGGeneratorContext ctx = SVGGeneratorContext.createDefault(doc);
        SVGTransform trans = new SVGTransform(ctx);
        s2.valueProperty().addListener(
                new ChangeListener<Number>() {
        
                    @Override
                    public void changed(ObservableValue<? extends Number> arg0,
                            Number oldValue, Number newValue) {

                        AffineTransform at = new AffineTransform();
                        at.rotate(newValue.doubleValue(), w, h);
                        GraphicContext gctx = new GraphicContext(at);
                        String matrix = trans.toSVGTransform(gctx);
                        //System.err.println("TRANSFORM: " + matrix);
                        e.setAttribute("transform",  matrix);

                        Image i = loader.getImage(doc);
                        iv.setImage(i);
                    }
                });

        Image i = loader.getImage(doc);
        iv.setImage(i);
*/
        /********** Simple animation sample */

        final Button b3 = new Button("Absolute");
        b3.translateXProperty().addListener(new ChangeListener<Number>() {

            @Override
            public void changed(ObservableValue<? extends Number> arg0,
                    Number oldValue, Number newValue) {
                x = newValue.doubleValue();
                b3.setText(String.format("Catch me if you can (%.1f/%.1f)", x, y));
            }
        });
        b3.translateYProperty().addListener(new ChangeListener<Number>() {

            @Override
            public void changed(ObservableValue<? extends Number> arg0,
                    Number oldValue, Number newValue) {
                y = newValue.doubleValue();
                b3.setText(String.format("Catch me if you can (%.1f/%.1f)", x, y));
            }
        });

        b3.setLayoutX(300);
        b3.setLayoutY(400);

        // Provides the animated scrolling behavior for the text
        TranslateTransition transTransition = new TranslateTransition(
                new Duration(75000), b3);
        transTransition.setToY(-820);
        transTransition.setToX(-820);
        transTransition.setInterpolator(Interpolator.LINEAR);
        transTransition.setCycleCount(Timeline.INDEFINITE);

        mainGroup.getChildren().add(grid);
        mainGroup.getChildren().add(b3);

        Scene scene = new Scene(mainGroup, 800, 600);
        //Image img = new Image("file:background2.png");
        //ImagePattern bg = new ImagePattern(img, 0, 0, img.getWidth(), img.getHeight(), false);
        //scene.setFill(bg);

        //SceneGraphTree tree = new SceneGraphTree();
        //tree.setSceneTree(scene);
        //tree.show();

        primaryStage.setScene(scene);
        primaryStage.show();

        transTransition.play();
    }

    private double x = 0, y = 0;

}
