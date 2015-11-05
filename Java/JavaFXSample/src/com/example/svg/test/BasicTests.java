package com.example.svg.test;

import static org.junit.Assert.*;
import javafx.scene.Group;
import javafx.scene.Node;

import org.junit.Test;

import com.example.svg.SVGLoader;

public class BasicTests {

    @Test
    public void testTigerImageBasics() {
        SVGLoader loader = new SVGLoader();
        Group svgImage = loader.loadSvg("Ghostscript_Tiger.svg");
        assertEquals(1, svgImage.getChildrenUnmodifiable().size());

        Node n = svgImage.getChildrenUnmodifiable().get(0);
        assertTrue(n instanceof Group);

        Group g = (Group) n;
        assertEquals(240, g.getChildrenUnmodifiable().size());
    }
}
