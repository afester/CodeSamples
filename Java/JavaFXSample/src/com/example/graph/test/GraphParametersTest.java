package com.example.graph.test;

import static org.junit.Assert.*;

import org.junit.Test;

import com.example.graph.GraphParameters;

public class GraphParametersTest {

    @Test
    public void testScaling() {
        GraphParameters gp = new GraphParameters(640, 480, 20, 20, 20, 20,
                                                 -50.0, 200.0, 
                                                 0.75, 40);
        assertEquals(128, gp.getX0());
        assertEquals(360, gp.getY0());
        assertEquals(2.56, gp.getScale(), 0.01);
        assertEquals(gp.getX0(), gp.mapToViewX(0), 0.01);
        assertEquals(gp.getY0(), gp.mapToViewY(0), 0.01);
        assertEquals(51, gp.getTickPixels());
        assertEquals(20.0, gp.getUnit(), 0.01);
    }
}
