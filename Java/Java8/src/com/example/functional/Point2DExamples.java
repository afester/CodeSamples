package com.example.functional;

import java.util.ArrayList;
import java.util.List;

import javafx.geometry.Point2D;

public class Point2DExamples {

    public static void main(String[] args) {
        new Point2DExamples()._main(args);
    }
    
    public void _main(String[] args) {
        List<Point2D> pointList = createPointList();
        
        System.err.println("Original list:");
        pointList.forEach(point -> System.err.println("  " + point));

        List<Point2D> sorted = new ArrayList<>();
        pointList.stream()
                 .sorted((p1, p2) -> Double.compare(p1.getY(), p2.getY()))
                 .forEach(e -> sorted.add(e));

        System.err.println("\nSorted by Y:");
        sorted.forEach(point -> System.err.println("  " + point));

        System.err.println("\nMax Y coordinate:");
        Point2D maxY = pointList.stream()
                                .max( (p1, p2) -> Double.compare(p1.getY(), p2.getY()) )
                                .get();
        System.err.println("  " + maxY);

        System.err.println("\nSorted by Y, then by X:");
        List<Point2D> sorted2 = new ArrayList<>();
        pointList.stream()
                 .sorted((p1, p2) -> {
                     final int yCompare = Double.compare(p1.getY(), p2.getY());
                     if (yCompare == 0) {
                         return Double.compare(p1.getX(), p2.getX());
                     }
                     return yCompare;
                 })
                 .forEach(e -> sorted2.add(e));
        sorted2.forEach(point -> System.err.println("  " + point));
        
        System.err.println("\nMax Y coordinate, then min X coordinate:");
        Point2D maxY2 = pointList.stream()
                                 .max( (p1, p2) -> {
                                    final int yCompare = Double.compare(p1.getY(), p2.getY());
                                    if (yCompare == 0) {
                                        return -Double.compare(p1.getX(), p2.getX());
                                    }
                                    return yCompare;
                                 })
                                .get();
        System.err.println("  " + maxY2);
        
        
        System.err.println("\nPairs of Points to draw lines between them:");
    }

    private List<Point2D> createPointList() {
        List<Point2D> points = new ArrayList<>();
        points.add(new Point2D(100, 100));
        points.add(new Point2D(120, 200));
        points.add(new Point2D(180, 130));
        points.add(new Point2D(90, 140));
        points.add(new Point2D(140, 50));
        points.add(new Point2D(80, 200));
        points.add(new Point2D(80, 40));
        points.add(new Point2D(130, 130));

        return points;
    }
}
