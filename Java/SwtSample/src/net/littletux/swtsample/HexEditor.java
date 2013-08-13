package net.littletux.swtsample;

import org.eclipse.swt.events.DisposeEvent;
import org.eclipse.swt.events.DisposeListener;
import org.eclipse.swt.events.PaintEvent;
import org.eclipse.swt.events.PaintListener;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.graphics.GC;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.widgets.Canvas;
import org.eclipse.swt.widgets.Composite;

public class HexEditor extends Canvas {

    public HexEditor(Composite parent, int style) {
        super(parent, style);

        // Add necessary listeners
        addDisposeListener(new DisposeListener() {
            public void widgetDisposed(DisposeEvent e) {
               HexEditor.this.widgetDisposed(e);
            }
        });
        addPaintListener(new PaintListener() {
            public void paintControl(PaintEvent e) {
               HexEditor.this.paintControl(e);
            }
        });

        redraw();
    }


    protected void widgetDisposed(DisposeEvent e) {
        System.err.println("DISPOSE EVENT ...");
    }

    public Point computeSize(int wHint, int hHint, boolean changed) {
        return new Point(50,50);
    }
    
    protected void paintControl(PaintEvent e) {
        System.err.println("PAINT EVENT ...");

        GC gc = e.gc;
        gc.setAntialias(1);
        gc.drawArc(10, 10, 50, 50, 0, 360);
    }
}
