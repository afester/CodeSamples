package com.example;

import java.util.HashMap;
import java.util.Map;

import javafx.scene.layout.GridPane;
import javafx.scene.paint.Color;
import javafx.scene.text.Text;

import com.example.graph.FunctionGraph;
import com.example.graph.Graph;

public class LegendTable extends GridPane {
    private Map<Graph, LegendTableRow> rows = new HashMap<>();
    private Functions plotter;

    public LegendTable(Functions plotter) {
        add(new Text("Function Graphs:"), 0, 0, 4, 1);
        setGridLinesVisible(true);
        this.plotter = plotter;
    }


    /**
     * Adds a new row to the legend grid.
     * 
     * @param graph
     */
    public void addLegendEntry(FunctionGraph graph) {
        LegendTableRow newRow = new LegendTableRow(this, graph);
        rows.put(graph, newRow);
    }

    public int getRowCount() {
        return rows.size();
    }

    public void removeLegend() {
        getChildren().clear();
        rows.clear();
    }

    
    public void setRowEdit(Graph graph) {
        LegendTableRow row = rows.get(graph);
        row.setEdit();
    }

    public void deleteRow(Graph graph) {
        System.err.println("DELETE");
        rows.remove(graph);
        plotter.removeGraph(graph);
    }

    public void commitRow(FunctionGraph graph, String newFormula, Color newColor) {
        plotter.setGraph(graph, newFormula, newColor);
    }

    
    /*
     * public void setError(int index, String s) { index--;
     * 
     * //Matcher matcher = pattern.matcher(s); //matcher.find();
     * errorLabels.get(index).setText(s); // "Error: " + matcher.group(1)); }
     */
}