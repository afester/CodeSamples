package com.example.rtree;

import javafx.geometry.BoundingBox;

/**
 */
public class RTreeSample {
    public static void main(String[] args) {
        RTree tree = new RTree();

        BoundingBox r1 = new BoundingBox(10, 10, 20, 20);
        BoundingBox r2 = new BoundingBox(15, 15, 20, 20);
        BoundingBox r3 = new BoundingBox(100, 100, 10, 10);
        tree.insertNode(r1);
        tree.insertNode(r2);
        tree.insertNode(r3);

        BoundingBox result = tree.hit(17,  17);
        System.err.println("RESULT:" + result);
    }
}
