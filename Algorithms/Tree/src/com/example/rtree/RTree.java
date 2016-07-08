package com.example.rtree;

import java.util.ArrayList;
import java.util.List;

import javafx.geometry.BoundingBox;

/**
 * 
 * - The R-tree is a balanced search tree (so all leaf nodes are at the same height)
 * - Insertion algorithm:
 *   - if the new object fits into the current node
 *     - if the current node overflows, split it
 *     - add new object to current node
 *   - 
 */
public class RTree {

    private static final int MAX_CAPACITY = 6;
    private static final int MIN_CAPACITY = 3;

    private BoundingBox rect;
    private List<RTree> children = new ArrayList<>();

    public void insertNode(BoundingBox node) {
        
    }


    public void removeNode(BoundingBox node) {

    }


    public BoundingBox hit(double x, double y) {

        if (rect.contains(x, y)) {
            if (children == null) {     // Leaf node
                return rect;
            }

            // search child nodes
            for (RTree child : children) {
                BoundingBox bb = child.hit(x, y);
                if (bb != null) {
                    return bb;
                }
            }
        }

        return null;
    }
}
