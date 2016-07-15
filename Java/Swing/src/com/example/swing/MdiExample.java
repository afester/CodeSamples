package com.example.swing;

import java.awt.BorderLayout;
import java.awt.Point;
import java.beans.PropertyVetoException;

import javax.swing.JButton;
import javax.swing.JDesktopPane;
import javax.swing.JFrame;
import javax.swing.JInternalFrame;
import javax.swing.JLabel;
import javax.swing.SwingUtilities;

public class MdiExample extends JFrame {


    private JDesktopPane Jdpmain;
    private Point loc = new Point(10, 10);

    private void addFrame() {
        JInternalFrame frm1 = new JInternalFrame();
        frm1.setVisible(true);
        frm1.setLocation(loc);
        loc.translate(10, 10);
        frm1.setSize(300, 200);

        addToContainer(frm1);

//        Jdpmain.add(frm1);
        try {
            frm1.setSelected(true);
        } catch (PropertyVetoException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }

    public void addToContainer(JInternalFrame internalframe){
        JInternalFrame []allFrames = Jdpmain.getAllFrames();//getting an array of all the frames in my jdesktoppane(jdpmain)
        boolean flag=false;
        for(int i=0;i<allFrames.length;i++){
            JInternalFrame currentFrame = allFrames[i];
            if(internalframe.getClass().isInstance(allFrames[i])){
                Jdpmain.setSelectedFrame(currentFrame);//if the internal frame already exist it might be under another jinternalframe so, I called this method to bring it to focus
                flag=true;
            }
            if(flag==false){
                Jdpmain.add(internalframe); //if an instance of the frame I am adding does not exist in jdesktoppane it should add an instance of it to jdesktoppane   
            } 
        }
    }

    private void start() {
        this.setLayout(new BorderLayout());
    
        Jdpmain = new JDesktopPane();

        JButton addBtn = new JButton("Add");
        addBtn.addActionListener(e -> addFrame());
        add(addBtn, BorderLayout.NORTH);
        add(Jdpmain, BorderLayout.CENTER);

        addFrame();

        setSize(640,  400);
        setLocation(200,  200);
        setVisible(true);
    }


    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new MdiExample().start());
    }
}
