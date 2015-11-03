package com.example.javafx.components;

import java.util.Arrays;

import javafx.beans.property.LongProperty;
import javafx.beans.property.SimpleLongProperty;
import javafx.event.Event;
import javafx.event.EventHandler;
import javafx.scene.Group;
import javafx.scene.Node;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;
import javafx.scene.shape.SVGPath;

import com.example.svg.SVGLoader;

/**
 * Implements the rendering of a single fourteen segment display.
 * The background of the display is transparent, so that it can be styled
 * from the containing component (usually a SevenSegmentPanel).
 */
public class FourteenSegment extends Group {

    private SVGPath panel = null;
    private SVGPath[] segments = new SVGPath[14];
    private SVGPath segDP = null;

    private Color onColor = Color.RED; //  new Color(0x23 / 255.0, 0x2A / 255.0, 0x47 / 255.0, 1.0);
    private Color offColor = Color.GREEN; // new Color(0xC9 / 255.0, 0xD3 / 255.0, 0xBB / 255.0, 1.0);
    private Color bgColor = Color.VIOLET;
    private int number;
    private boolean isDp;

    private LongProperty currentMaskProperty = new SimpleLongProperty(0);
    public LongProperty getCurrentMaskProperty() { return currentMaskProperty; }
    public long getCurrentMask() { return currentMaskProperty.get(); }
    public void setCurrentMask(long value) { currentMaskProperty.set(value); }
    
    private final static boolean[][] digits = new boolean[][] {
        //     a
        //    __
        //  f| g|  b
        //    --
        //  e|__|  c
        //     d
        //
        // a     b      c      d      e      f      g1      g2     h       i      j       k      l       m
        {true,  true,  true,  true,  true,  true,  false,  false, false, false, true,  false, false,  true}, // 0
        {false, true,  true,  false, false, false, false,  false, false, false, false,  false, false,  false}, //1
        {true,  true,  false, true,  true,  false, true ,  true, false, false, false,  false, false,  false}, // 2
        {true,  true,  true,  true,  false, false, false,  true, false, false, false,  false, false,  false}, // 3
        {false, true,  true,  false, false, true,  true ,  true, false, false, false,  false, false,  false}, // 0
        {true,  false, true,  true,  false, true,  true ,  true, false, false, false,  false, false,  false}, // 0
        {true,  false, true,  true,  true,  true,  true ,  false, false, false, false,  false, false,  false}, // 0
        {true,  true,  true,  false, false, false, false,  false, false, false, false,  false, false,  false}, // 0
        {true,  true,  true,  true,  true,  true,  true ,  false, false, false, false,  false, false,  false}, // 0
        {true,  true,  true,  true,  false, true,  true ,  false, false, false, false,  false, false,  false}, // 0

        {true,  true,  true,  false, true,  true,  true ,  false, false, false, false,  false, false,  false}, // 0
        {false, false, true,  true,  true,  true,  true ,  false, false, false, false,  false, false,  false}, // 0
        {true,  false, false, true,  true,  true,  false,  false, false, false, false,  false, false,  false}, // 0
        {false, true,  true,  true,  true,  false, true ,  false, false, false, false,  false, false,  false}, // 0
        {true,  false, false, true,  true,  true,  true ,  false, false, false, false,  false, false,  false}, // 0
        {true,  false, false, false, true,  true,  true ,  false, false, false, false,  false, false,  false}, // 0

        {false, false, false, false, true,  false, true ,  false, false, false, false,  false, false,  false}, // 0
        {false, false, true,  true,  true,  false, false,  false, false, false, false,  false, false,  false}, // 0
        {false, false, false, false, true,  true,  false,  false, false, false, false,  false, false,  false}, // 0
        {false, false, true,  true,  true,  false, false,  false, false, false, false,  false, false,  false}, // 0
        {false, false, false, false, false, false, false,  false, false, false, false,  false, false,  false}, // 0
        {false, true,  true,  false, true,  true,  true ,  false, false, false, false,  false, false,  false}, // 0
        {false, false, false, true,  true,  true,  false,  false, false, false, false,  false, false,  false}, // 0
        {true,  true,  false, false, true,  true,  true ,  false, false, false, false,  false, false,  false}, // 0
        {false, true,  false, false, true,  true,  true ,  false, false, false, false,  false, false,  false}, // 0
        {false, false, true,  false, true,  false, true ,  false, false, false, false,  false, false,  false} // 0
    };


    //     a
    //    -- --
    //  f|\ | /|
    //  f| \|/ |
    //    -- --
    //   | /|\ |
    //  e|/ | \|
    //    -- --

    private final static long segmentTable[] = {
        0b0000000000000000,
        0b0000000000000001,
        0b0000000000000010
    };


    int idx = 0;
    public FourteenSegment() {

        // load the drawing
        SVGLoader loader = new SVGLoader();
        Node g = loader.loadSvg("14segment.svg");
        getChildren().add(g);

        final String[] ids = {"segA", "segB", "segC", "segD", "segE", "segF",
                              "segG1", "segG2", "segH", "segI", "segJ", "segK", "segL", "segM"};
        for (String id : ids) {
            segments[idx] = (SVGPath) g.lookup("#" + id);
            segments[idx].setOnMouseClicked(new EventHandler<MouseEvent>() {
                final int segIdx;  { segIdx = idx; }

                @Override
                public void handle(MouseEvent event) {
                    setSegmentEnabled(segIdx, !isSegmentEnabled(segIdx));
                }
            });

            idx++;
        }
        segDP = (SVGPath) g.lookup("#segDP");
        panel = (SVGPath) g.lookup("#panel");

        // initialize default colors
        panel.setFill(new Color(1.0, 1.0, 1.0, 0.0));
                                // NOTE/TODO: Need a fill color here, otherwise
                                // the bounds of the panel shape are not considered
                                // when layouting! i.e. setFill(null) will cause
                                // the panel shape to not be rendered at all
        clear();
        setDP(false);
        
        currentMaskProperty.addListener(e -> {
            long mask = ((LongProperty) e).getValue();

            long bit = 1;
            for (int s = 0;  s < segments.length;  s++) {
                if ((mask & bit) != 0) {
                    segments[s].setFill(onColor);
                } else {
                    segments[s].setFill(offColor);
                }
                bit <<= 1;
            }
        });
    }



    public Color getOnColor() {
        return onColor;
    }

    /**
     * Sets the color to use for enabled segments.
     *
     * @param color The color to use when painting enabled segments.
     */
    public void setOnColor(Color col) {
        onColor = col;
        setDigit(this.number);
        setDP(this.isDp);
    }

    public Color getOffColor() {
        return offColor;
    }

    /**
     * Sets the color to use for disabled segments.
     *
     * @param color The color to use when painting disabled segments.
     */
    public void setOffColor(Color col) {
        offColor = col;
        setDigit(this.number);
        setDP(this.isDp);
    }

    
    public void setSegmentEnabled(int idx, boolean state) {
        long segMask = 1 << idx;
        if (state) {
            segments[idx].setFill(onColor);
            setCurrentMask(getCurrentMask() | segMask); 
        } else {
            segments[idx].setFill(offColor);
            setCurrentMask(getCurrentMask() & ~segMask);
        }
    }


    public boolean isSegmentEnabled(int idx) {
        long segMask = 1 << idx;
        return (getCurrentMask() & segMask) != 0; 
    }

    

    /**
     * Enables the necessary digits to show a (hexadecimal) number from 
     * 0 to f (15d).
     * 
     * @param number The number to show (0 to 15)
     */
    public void setDigit(int number) {
        this.number = number;
        for (int s = 0;  s < segments.length;  s++) {
            setSegmentEnabled(s, digits[number][s]);
        }
    }


    /**
     * Shows a particular character in the display.
     * Currently, the display supports numbers from 0 to 9 and the characters
     * <code>abcdef</code> (shown in either capital or non-capital depending on the 
     * actual character - e.g. 'a' is always displayed as "A", independant whether
     * it is passed as 'a' or 'A').
     * Additionally, the characters <code>HlLnoOPrvµ</code> and the space character
     * are supported. The capital 'O' is shown identical to the number 0,
     * and the rendering of the 'v' looks very bad.
     *
     * @param c The character to display.
     * @throws RuntimeException if the character is not supported. Some
     *         characters (such as M or Z) can not be displayed with seven segments
     *         in a useful way. Use an 14 segment display instead. 
     */
    public void setChar(char c) {
        if (c < 32 || c >= 128) {
            throw new RuntimeException("Error: invalid character '" + c + "'");
        }

        // " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
        int hexVal = Character.digit(c, 16);
        if (hexVal >= 0) {
            setDigit(hexVal);
        } else {
            switch(c) {
                // TODO: Use Map
                case 'O' : setDigit(0);   break;
                case 'r' : setDigit(16);  break;
                case 'v' : setDigit(17);  break;
                case 'l' : setDigit(18);  break;
                case 'o' : setDigit(19);  break;
                case ' ' : setDigit(20);  break;
                case 'H' : setDigit(21);  break;
                case 'L' : setDigit(22);  break;
                case 'P' : setDigit(23);  break;
                case 'µ' : setDigit(24);  break;

                default:
                    throw new RuntimeException("Error: invalid character '" + c + "'");
            }
        }
    }


    /**
     * Turns off all seven segments. The decimal point is not affected by this
     * method - to disable all segments, use clear() and setDP(false).
     */
    public void clear() {
        for (SVGPath seg : segments) {
            seg.setFill(offColor);
        }
        this.number = 20; // space ; TODO!
    }

    /**
     * Enables or disabled the decimal point for this segment.
     * 
     * @param state <code>true</code> to enable the decimal point, 
     *              <code>false</code> to disable it.
     */
    public void setDP(boolean state) {
        this.isDp = state;
        if (state) {
            segDP.setFill(onColor);    
        } else {
            segDP.setFill(offColor);
        }
    }
/*
    public void setBackgroundColor(Color newPaint) {
        SevenSegment.this.panel.setFill(newPaint);        
    }*/
}
