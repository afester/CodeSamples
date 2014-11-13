package com.example.graph;

import javafx.geometry.Point2D;

/**
 * A general set of parameters for drawing any kind of graphs
 * (limited to linear scaling currently)
 */
public class GraphParameters {
    // given parameters
    int canvasWidth = 0;
    int canvasHeight = 0;
    int minLeftMargin = 0;
    int minRightMargin = 0;
    int minTopMargin = 0;
    int minBottomMargin = 0;
    double fromX = 0;
    double toX = 0;
    double y0Location = 0;
    int minScalePixels = 0;

    // calculated parameters
    int x0 = 0;
    int y0 = 0;
    double fromY = 0;
    double toY = 0;
    double scale;
    double unit;
    int tickPixels = 0;


    public GraphParameters(int canvasWidth, int canvasHeight,
                           int minLeftMargin, int minRightMargin, int minTopMargin, int minBottomMargin,
                           double fromX, double toX, double y0Location,
                           int minScalePixels) {
        this.canvasWidth = canvasWidth;
        this.canvasHeight = canvasHeight;
        this.minLeftMargin = minLeftMargin;
        this.minRightMargin = minRightMargin;
        this.minTopMargin = minTopMargin;
        this.minBottomMargin = minBottomMargin;
        this.fromX = fromX;
        this.toX = toX;
        this.y0Location = y0Location;
        this.minScalePixels = minScalePixels;

        recalculate();
    }


    private void recalculate() {

        x0 = (int) ((Math.abs(fromX) / (Math.abs(fromX) + Math.abs(toX))) * canvasWidth);
        if (x0 < minBottomMargin) {
            x0 = minTopMargin;
        }
        if (x0 > canvasWidth - minLeftMargin) {
            x0 = canvasWidth - minLeftMargin; 
        }

        scale = canvasWidth / (toX - fromX);

        y0 = (int) (y0Location * canvasHeight);
        if (y0 < minTopMargin) {
            y0 = minTopMargin;
        }
        if (y0 > canvasHeight - minBottomMargin) {
            y0 = canvasHeight - minBottomMargin;
        }
        toY = y0 / scale;
        fromY = (y0 - canvasHeight) / scale;

        // simple algorithm to calculate a reasonable scale for the axis ticks
        double scaleTemp = minScalePixels / scale;
        int magnitude = 1;
        while(!(scaleTemp < 10)) {
            scaleTemp = scaleTemp / 10;
            magnitude = magnitude * 10;
        }

        unit= 1;
        if (scaleTemp < 1) {
            unit = 1;
        } else if (scaleTemp < 2) {
            unit = 2;
        } else if (scaleTemp < 5) {
            unit = 5;
        } else {
            unit = 10;
        }
        unit = unit * magnitude;
        tickPixels = (int) (unit * scale);
    }

    public int getLeftMargin() {
        return minLeftMargin;
    }

    public int getRightMargin() {
        return minRightMargin;
    }

    public int getTopMargin() {
        return minTopMargin;
    }

    public int getBottomMargin() {
        return minBottomMargin;
    }
    
    public double mapToViewX(double xval) {
        return (int) (x0 + xval * scale);
    }
    

    public double mapToViewY(double yval) {
        return (y0 - yval * scale); 
    }

    public Point2D mapToView(Point2D point) {
        return new Point2D(mapToViewX(point.getX()), mapToViewY(point.getY()));
    }

    
    
    public int getCanvasWidth() {
        return canvasWidth;
    }
    
    
    public int getCanvasHeight() {
        return canvasHeight;
    }

    public int getY0() {
        return y0;
    }
    
    
    public int getX0() {
        return x0;
    }
    
    
    public double getFromX() {
        return fromX;
    }

    
    public double getToX() {
        return toX;
    }


    public double getFromY() {
        return fromY;
    }

    
    public double getToY() {
        return toY;
    }

    public double getScale() {
        return scale;
    }

    /**
     * @return The number of pixels between each tick on the axes in view coordinates
     */
    public int getTickPixels() {
        return tickPixels;
    }

    /**
     * @return The difference between each tick on the axes in model coordinates
     */
    public double getUnit() {
        return unit;
    }


    public void setRange(double startX, double stopX) {
        fromX = startX;
        toX = stopX;
        recalculate();
    }


}
