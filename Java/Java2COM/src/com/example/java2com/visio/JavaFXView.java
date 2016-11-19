package com.example.java2com.visio;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.example.java2com.IDispatch;
import com.example.java2com.Variant;
import com.example.java2com.VariantOut;

import javafx.application.Application;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.Point2D;
import javafx.geometry.Pos;
import javafx.scene.Group;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.Button;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.Slider;
import javafx.scene.layout.GridPane;
import javafx.scene.paint.Color;
import javafx.scene.text.Text;
import javafx.scene.transform.Scale;
import javafx.scene.transform.Shear;
import javafx.stage.Stage;

public class JavaFXView extends Application {

    private int rowCount = 0;
    private GridPane grid = new GridPane();
    private Canvas canvas; 
    private GraphicsContext context ;

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

    private Pattern pattern = Pattern.compile("RGB\\((\\d*); (\\d*); (\\d*)\\)");

    private Color parseRGB(String s) {
        Color result;
        try {
            Matcher matcher = pattern.matcher(s);
            matcher.find();
            int r = Integer.parseInt(matcher.group(1));
            int g = Integer.parseInt(matcher.group(2));
            int b = Integer.parseInt(matcher.group(3));
            result = new Color(r/255F, g/255F, b/255F, 1.0);
        }catch(IllegalStateException ise) {
            System.err.println("Invalid Color: "  + s);
            result = Color.BLACK;
        }

        return result;
    }

    private IVDocument document;

    private void render(String pageName) {
        IVPages pages = document.getPages();
        IVPage page = pages.getItem(new Variant(pageName));

        // get width and height of the page
        IVShape properties = page.getPageSheet();

        // http://msdn.microsoft.com/en-us/library/bb267452(v=office.12).aspx
        double w = properties.getCells("PageWidth").getResult(new Variant("mm"));
        double h = properties.getCells("PageHeight").getResult(new Variant("mm"));
        canvas.setWidth(w);
        canvas.setHeight(h);

        context.clearRect(0, 0, canvas.getWidth(), canvas.getHeight());
        context.setFill(new Color(255F/255F, 255F/255F, 204F/255F, 1.0));
        context.fillRect(0,  0, canvas.getWidth(), canvas.getHeight());

        IVShapes shapes = page.getShapes();

        //for (Shape s : shapes) {
        for (int i = 1;  i <= shapes.getCount();  i++) {
            IVShape s = shapes.getItem(new Variant(i));
            System.out.println(s);

            String col = s.getCells("LineColor").getResultStr(new Variant(""));
            Color lineColor = parseRGB(col);

            long linePattern = s.getCells("LinePattern").getResultInt(new Variant(""), 0);
            long fillPattern = s.getCells("FillPattern").getResultInt(new Variant(""), 0);

            col = s.getCells("FillBkgnd").getResultStr(new Variant(""));
            Color fillColor = parseRGB(col);

            col = s.getCells("FillForegnd").getResultStr(new Variant(""));
//            Color foreColor = parseRGB(col);

            if (s.getName().startsWith("Circle") || s.getName().startsWith("Ellipse")) {
                double radiusX = (s.getCells("Width").getResult(new Variant("mm")))/2;
                double radiusY = (s.getCells("Height").getResult(new Variant("mm")))/2;
                double xPos = s.getCells("PinX").getResult(new Variant("mm"));
                double yPos = h - s.getCells("PinY").getResult(new Variant("mm"));

                context.beginPath();

                context.setLineWidth(1);
                context.setFill(fillColor);
                context.setStroke(lineColor);

                context.arc(xPos,  yPos,  radiusX,  radiusY,  0,  1000.0);

                if (fillPattern > 0) {
                    context.fill();
                }
                if (linePattern > 0) {
                    context.stroke();
                }
            } else {
                IVPaths paths = s.getPaths();

                for (IVPath path : paths) {
                    System.err.println("   " + path);
                    context.beginPath();

                    context.setLineWidth(1);
                    context.setFill(fillColor);
                    context.setStroke(lineColor);

                    boolean first = true;
                    //for (Curve c : path) {
                    for (int ci = 1;  ci < path.getCount();  ci++) {
                        IVCurve c = path.getItem(ci);

                        if (first) {
                            double start = c.getStart();

                            // Point2D p1 = c.point(start);
                            VariantOut x = new VariantOut(VariantOut.VT_R8);
                            VariantOut y = new VariantOut(VariantOut.VT_R8);
                            c.point(start, x, y);
                            Point2D p1 = new Point2D(x.doubleValue, y.doubleValue);

                            p1 = p1.multiply(25.4);
                            p1 = new Point2D(p1.getX(), h - p1.getY());
                            context.moveTo(p1.getX(), p1.getY());
                            first = false;
                        }
                        double end = c.getEnd();
        
                        // Point2D p2 = c.point(end);
                        VariantOut x = new VariantOut(VariantOut.VT_R8);
                        VariantOut y = new VariantOut(VariantOut.VT_R8);
                        c.point(end, x, y);
                        Point2D p2 = new Point2D(x.doubleValue, y.doubleValue);

                        p2 = p2.multiply(25.4);
                        p2 = new Point2D(p2.getX(), h - p2.getY());
        
                        context.lineTo(p2.getX(), p2.getY());
                    }

                    if (fillPattern > 0) {
                        context.fill();
                    }
                    if (linePattern > 0) {
                        context.stroke();
                    }
                }
            }

            s.release();
        }

        shapes.release();
        page.release();
        pages.release();
    }

    @Override
    public void start(Stage primaryStage) {
        primaryStage.setTitle("JavaFX viewer for Visio");

        final Group mainGroup = new Group();

        grid.setAlignment(Pos.TOP_LEFT);
        // grid.setGridLinesVisible(true);

        
        String projDir = System.getProperty("user.dir");
        String sampleFile = projDir + "/Sample.vsd";
        
        canvas = new Canvas(300, 400);
 //       canvas.getTransforms().add(new Shear(-0.2, 0.0));
 //       canvas.getTransforms().add(new Scale(2.0, 2.0));

        context = canvas.getGraphicsContext2D();

        final ChoiceBox<String> pageSelect = new ChoiceBox<>();
        document = IDispatch.get(sampleFile, IVDocument.class);
        //for (Page p : document.getPages()) {
        IVPages pages = document.getPages();
        for (int ip = 1;  ip <= pages.getCount();  ip++) {
            IVPage p = pages.getItem(new Variant(ip));
            System.err.println(p.getName());
            pageSelect.getItems().add(p.getName());
        }

        Button refresh = new Button("Refresh");
        refresh.setOnAction(new EventHandler<ActionEvent>() {
            @Override
            public void handle(ActionEvent arg0) {
                String pageName = pageSelect.getSelectionModel().getSelectedItem();
                System.err.println("Refresh from: " + pageName);
                render(pageName);
            }
        });

        Slider s1 = new Slider(0.0, 1.0, 1.0);
        s1.valueProperty().addListener(
            new ChangeListener<Number>() {
    
                @Override
                public void changed(ObservableValue<? extends Number> arg0,
                        Number oldValue, Number newValue) {
                    //g1.setOpacity(newValue.doubleValue());
                }
            }
        );

        addRow(new Text("Page:"), pageSelect);
        addRow(new Text("Scale:"), s1);
        // addRow(new Text("Shear:"), pageSelect);
        addRow(refresh);
        addRow(canvas);

        mainGroup.getChildren().add(grid);

        Scene scene = new Scene(mainGroup, 800, 600);
        primaryStage.setScene(scene);

        primaryStage.show();
    }
}
