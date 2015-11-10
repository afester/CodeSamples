package com.example.svg;

import javafx.scene.effect.MotionBlur;

import org.apache.batik.anim.dom.SVGOMFECompositeElement;
import org.apache.batik.anim.dom.SVGOMFEGaussianBlurElement;
import org.apache.batik.anim.dom.SVGOMFEMergeElement;
import org.apache.batik.anim.dom.SVGOMFEMergeNodeElement;
import org.apache.batik.anim.dom.SVGOMFEOffsetElement;
import org.apache.batik.anim.dom.SVGOMFEPointLightElement;
import org.apache.batik.anim.dom.SVGOMFESpecularLightingElement;
import org.apache.batik.anim.dom.SVGOMFilterElement;

import com.sun.scenario.effect.GaussianBlur;

public class SVGFilterHandler {

    public void handleElement(SVGOMFilterElement e) {
        System.err.println("Defining new filter: " + e.getId());
    }

    public void handleElement(SVGOMFEGaussianBlurElement e) {
        System.err.println("Adding GaussianBlur filter: ");
        System.err.println("   In=" + e.getIn1().getBaseVal());
        System.err.println("   StdDeviation=" + e.getAttribute("stdDeviation")); //  .getStdDeviationX() + "/" + e.getStdDeviationY()); // NYI
        System.err.println("   Result=" + e.getResult().getBaseVal());

        GaussianBlur mb = new GaussianBlur();
    }

    public void handleElement(SVGOMFEOffsetElement e) {
    }

    public void handleElement(SVGOMFESpecularLightingElement e) {
    }

    public void handleElement(SVGOMFEPointLightElement e) {
    }

    public void handleElement(SVGOMFECompositeElement e) {
    }

    public void handleElement(SVGOMFEMergeNodeElement e) {
    }

    public void handleElement(SVGOMFEMergeElement e) {
    }

}
