package com.example;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;

import java.util.Map;

class SparseArray2D<T> {
    private int colCount = 0;
    private int rowCount = 0;
    private Map<Integer, Map<Integer, T>> theMap = new HashMap<Integer, Map<Integer, T>>();

    public SparseArray2D() {
        
    }

    
    public void put(int row, int col, T obj) {
        Map<Integer, T> theRow = theMap.get(row);
        if (theRow == null) {
            theRow = new HashMap<Integer, T>();
            theMap.put(row, theRow);
        }
        theRow.put(col, obj);

        colCount = Math.max(colCount, col+1);
        rowCount = Math.max(rowCount, row+1);
    }


    public T get(int row, int col) {
        Map<Integer, T> theRow = theMap.get(row);
        if (theRow != null) {
            return theRow.get(col);    
        }
        return null;
    }

    
    public String toString() {
        StringBuffer result = new StringBuffer();
        result.append("[");

        for (int row = 0; row < rowCount;  row++) {
            if (row > 0) {
                result.append("\n [");
            } else {
                result.append('[');
            }

            for (int col = 0;  col < colCount; col++) {
                if (col > 0) {
                    result.append(", ");
                }

                T value = get(row, col);
                if (value != null) {
                    result.append(String.format("%5s", value));
                } else {
                    result.append(" null");
                }
            }
            result.append(']');
        }

        result.append("]");
        return result.toString();
    }
}

