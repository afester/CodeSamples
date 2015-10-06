package com.example.javafx.components;

import javafx.scene.control.SkinBase;



public class SevenSegmentPanelSkin extends SkinBase<SevenSegmentPanel> {

    @Override
    protected double computeMaxWidth(double height, double topInset,
            double rightInset, double bottomInset, double leftInset) {
        return computePrefWidth(height, topInset, rightInset, bottomInset, leftInset);
    }

    @Override
    protected double computeMaxHeight(double width, double topInset,
            double rightInset, double bottomInset, double leftInset) {
        return computePrefHeight(width, topInset, rightInset, bottomInset, leftInset);
    }

    public SevenSegmentPanelSkin(SevenSegmentPanel control) {
        super(control);
    }
}
