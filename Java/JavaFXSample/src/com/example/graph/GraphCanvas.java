package com.example.graph;

import java.util.ArrayList;
import java.util.List;

import javafx.geometry.Point2D;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;

public class GraphCanvas extends Canvas {

    private GraphParameters gp;
    private GraphicsContext context;
    private List<Graph> graphs = new ArrayList<>();

    public GraphCanvas(GraphParameters gp) {
        super(gp.canvasWidth, gp.canvasHeight);
        this.gp = gp;
        context = getGraphicsContext2D();
    }

    private void drawGraph(Graph theGraph) {
        // do not draw the graph if it is in an error state
        if (theGraph.isError()) {
            return;
        }

        context.beginPath();
        context.setStroke(theGraph.getColor());
        context.setLineWidth(1);
        // FGV.context.setLineDash([]);

        // theGraph.setT(this.t);

        Point2DOperationIterator iter = theGraph.operationIterator(gp.getFromX(), gp.getToX(), 1 / gp.getScale());
        while(iter.hasNext()) {
            Point2DOperation p = iter.next();
            Point2D viewCoords = gp.mapToView(p.getPoint());
            if (p.getOperation() == Point2DOperation.Operation.MOVETO) {
                context.moveTo(viewCoords.getX() + 0.5, viewCoords.getY() + 0.5);
            } else {
                context.lineTo(viewCoords.getX() + 0.5, viewCoords.getY() + 0.5);
            }
        }

/*        
        
        for (double x = gp.getFromX(); x < gp.getToX(); x += scalePerPixel) {
            try {
                Double y = theGraph.evaluate(x);

                if (!Double.isNaN(y)) {

                    double xpos = gp.toViewX(x);
                    double ypos = gp.toViewY(y);
                    if (move) {
                        move = false;
                        context.moveTo(xpos, ypos);
                    } else {
                        context.lineTo(xpos, ypos);
                    }
                } else {
                    move = true; // next non-NaN needs to MOVE first
                }
            } catch (Exception e) {
                e.printStackTrace();
                // legendTable.setError(theGraph.getIndex(),
                // e.getLocalizedMessage());
                theGraph.setIsError(true);
                return;
            }
        }
*/
        context.stroke();
    }

    public void renderScene() {
        context.clearRect(0, 0, getWidth(), getHeight());

        // draw the axes
        drawAxis();

        // draw the graphs
        for (Graph graph : graphs) {
            drawGraph(graph);
        }
    }

    /**
     * @param y
     *            tick position in model coordinates
     */
    private void drawYTick(double y) {
        double ypos = gp.mapToViewY(y);

        if (ypos > 20) { // do not draw a tick nearby the end of the axis!
            context.moveTo(gp.getX0() - 5, ypos + 0.5);
            context.lineTo(gp.getX0() + 5, ypos + 0.5);

            String value = "" + Math.round(y); // * 10) / 10.0;
            double textWidth = 15; // / context.measureText(value).width;
            context.fillText(value, gp.getX0() - textWidth - 7, ypos + 3);
        }
    }

    /**
     * @param x
     *            tick position in model coordinates
     */
    private void drawXTick(double x) {
        double xpos = gp.mapToViewX(x);

        context.moveTo(xpos + 0.5, gp.getY0() - 5);
        context.lineTo(xpos + 0.5, gp.getY0() + 5);

        String value = "" + Math.round(x); // * 10) / 10.0;
        double textWidth = 15; // / FGV.context.measureText(value).width;
        context.fillText(value, xpos - textWidth / 2, gp.getY0() + 15);
    }

    /**
     * Draws the axes including the ticks.
     */
    private void drawAxis() {
        // set the axis visual properties
        context.beginPath();
        context.setStroke(Color.BLACK);
        context.setLineWidth(1);
        // FGV.context.setLineDash([]);

        // draw the axis lines

        // Y axis
        context.moveTo(gp.getX0() + 0.5, getHeight() + 0.5);
        context.lineTo(gp.getX0() + 0.5, 0.5);

        // X axis
        context.moveTo(gp.getLeftMargin() + 0.5, gp.getY0() + 0.5);
        context.lineTo(getWidth() + 0.5, gp.getY0() + 0.5);

        double tick = gp.getUnit();

        // draw the y scale
        for (double y = -tick; y > gp.getFromY(); y -= tick) { // negative axis
            drawYTick(y);
        }
        for (double y = tick; y < gp.getToY(); y += tick) { // positive axis
            drawYTick(y);
        }

        // draw the x scale
        for (double x = -tick; x > gp.getFromX(); x -= tick) { // negative axis
            drawXTick(x);
        }
        for (double x = tick; x < gp.getToX(); x += tick) { // positive axis
            drawXTick(x);
        }

        context.stroke();
    }

    public void addGraph(Graph graph) {
        this.graphs.add(graph);
    }

}
