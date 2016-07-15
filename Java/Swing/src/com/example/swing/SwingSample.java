package com.example.swing;

import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;

import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

public class SwingSample extends JFrame {

    protected void addComponent(int axis, int direction, int insetOne, int insetTwo, int insetThree, 
                                int insetFour, int gridX, int gridY, int gridWidth, JComponent name) {

        GridBagConstraints bc = new GridBagConstraints();
    
        bc.fill = axis;
        bc.anchor = direction;
        bc.insets = new Insets(insetOne, insetTwo, insetThree, insetFour);
        bc.gridx = gridX;
        bc.gridy = gridY;
        bc.gridwidth = gridWidth;
    
        this.add(name, bc);
    }


    private void start() {
        GridBagLayout gridbag = new GridBagLayout();
        this.setLayout(gridbag);
    
        addComponent(GridBagConstraints.HORIZONTAL, GridBagConstraints.WEST, 0, 10, 10, 0, 0, 0, 1, new JLabel("Hello"));
        addComponent(GridBagConstraints.HORIZONTAL, GridBagConstraints.WEST, 0, 10, 10, 0, 0, 1, 1, new JButton("World"));

        setSize(640,  400);
        setLocation(200,  200);
        setVisible(true);
    }


    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new SwingSample().start());
    }
}
