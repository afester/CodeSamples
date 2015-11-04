package com.example.svg;

import java.awt.geom.AffineTransform;
import java.awt.geom.Rectangle2D;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.net.MalformedURLException;

import javafx.embed.swing.SwingFXUtils;
import javafx.scene.Group;
import javafx.scene.Node;
import javafx.scene.image.Image;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;
import javafx.scene.paint.Paint;
import javafx.scene.shape.CubicCurve;
import javafx.scene.shape.Line;
import javafx.scene.shape.Rectangle;
import javafx.scene.shape.SVGPath;
import javafx.scene.shape.Shape;
import javafx.scene.text.Font;
import javafx.scene.text.Text;
import javafx.scene.transform.Affine;

import org.apache.batik.anim.dom.SAXSVGDocumentFactory;
import org.apache.batik.anim.dom.SVGDOMImplementation;
import org.apache.batik.anim.dom.SVGOMAnimatedPathData;
import org.apache.batik.anim.dom.SVGOMAnimatedPathData.BaseSVGPathSegList;
import org.apache.batik.anim.dom.SVGOMDefsElement;
import org.apache.batik.anim.dom.SVGOMDocument;
import org.apache.batik.anim.dom.SVGOMGElement;
import org.apache.batik.anim.dom.SVGOMMetadataElement;
import org.apache.batik.anim.dom.SVGOMPathElement;
import org.apache.batik.anim.dom.SVGOMPatternElement;
import org.apache.batik.anim.dom.SVGOMRectElement;
import org.apache.batik.anim.dom.SVGOMSVGElement;
import org.apache.batik.anim.dom.SVGOMTSpanElement;
import org.apache.batik.anim.dom.SVGOMTextElement;
import org.apache.batik.anim.dom.SVGStylableElement;
import org.apache.batik.anim.dom.SVGStylableElement.PresentationAttributePaintValue;
import org.apache.batik.anim.dom.SVGStylableElement.PresentationAttributeValue;
import org.apache.batik.css.dom.CSSOMSVGStyleDeclaration;
import org.apache.batik.css.dom.CSSOMSVGStyleDeclaration.StyleDeclarationPaintValue;
import org.apache.batik.css.dom.CSSOMStyleDeclaration.StyleDeclarationValue;
import org.apache.batik.css.engine.CSSContext;
import org.apache.batik.css.engine.CSSEngine;
import org.apache.batik.css.engine.value.Value;
import org.apache.batik.dom.GenericComment;
import org.apache.batik.dom.GenericElementNS;
import org.apache.batik.dom.GenericText;
import org.apache.batik.dom.svg.SVGContext;
import org.apache.batik.dom.svg.SVGPathSegItem;
import org.apache.batik.transcoder.TranscoderException;
import org.apache.batik.transcoder.TranscoderInput;
import org.apache.batik.transcoder.TranscoderOutput;
import org.apache.batik.transcoder.image.ImageTranscoder;
import org.apache.batik.util.ParsedURL;
import org.apache.batik.util.XMLResourceDescriptor;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
import org.w3c.dom.css.CSSPrimitiveValue;
import org.w3c.dom.css.CSSStyleDeclaration;
import org.w3c.dom.svg.SVGMatrix;
import org.w3c.dom.svg.SVGPaint;
import org.w3c.dom.svg.SVGTransform;
import org.w3c.dom.svg.SVGTransformList;
import org.w3c.dom.svg.SVGTransformable;

import com.sun.glass.ui.Screen;

class BufferedImageTranscoder extends ImageTranscoder {

    private BufferedImage img = null;

    @Override
    public BufferedImage createImage(int width, int height) {
        BufferedImage bi = new BufferedImage(width, height, BufferedImage.TYPE_INT_ARGB);
        return bi;
    }

    @Override
    public void writeImage(BufferedImage img, TranscoderOutput to) throws TranscoderException {
        this.img = img;
    }

    public BufferedImage getBufferedImage() {
        return img;
    }
}


public class SVGLoader implements CSSContext {

    private Group parentNode;
    private Color parentFill = null;    // TODO: HACK!
    private Color parentStroke = null;    // TODO: HACK!
    
    private Group result;
    
    private boolean addRootRect = false;    // flag whether to add a rectangle in the size of the drawing


    public SVGLoader() {
    }


    /**
     * @param fileName The name of the SVG file to load.
     *
     * @return A JavaFX node representing the SVG file.
     */
    public Group loadSvg(String fileName) {
        // note: uses the DOM approach.
        // probably a SAX based approach would be better from a performance perspective.
        SVGOMDocument doc = (SVGOMDocument) loadSvgDocument(fileName);

        result = new Group();
        parentNode = result;
        handle(doc);
        return result;
    }


    /**
     * @param flag Flag to determine whether or not to add a rectangle in the 
     *             size of the drawing's viewbox.
     */
    public void setAddViewboxRect(boolean flag) {
        this.addRootRect = flag;
    }

    


    private String indent(int level) {
        return "                                    ".substring(0, level*2);
    }

    
    private int level = 0;
    
    private void handle(org.w3c.dom.Node node) {
/*        if (node instanceof SVGElement) {
            SVGElement e = (SVGElement) node;
            System.err.println("NODE TYPE: " + e.getNodeType());
        } else {
            System.err.println("NOPE.");
        }
*/
        Node result = null;

        Group par = parentNode;  // save current parent
        Color parFill = parentFill;
        Color parStroke = parentStroke;

        // System.err.printf("%s%s\n", indent(level), node.getClass());

        if (node instanceof SVGOMDocument) {
            SVGOMDocument obj = (SVGOMDocument) node;
            //System.err.printf("%s%s\n", indent(level), "SVGOMDocument");

        // The <svg> tag
        } else if (node instanceof SVGOMSVGElement) {
            SVGOMSVGElement obj = (SVGOMSVGElement) node;

            // optionally add a rectangle using the size of the whole drawing
            if (addRootRect) {
                float height = obj.getViewBox().getBaseVal().getHeight();
                float width = obj.getViewBox().getBaseVal().getWidth();
                result = new Rectangle(width,  height, null);
                ((Rectangle)result).setStroke(Color.BLACK);
                ((Rectangle)result).getStrokeDashArray().addAll(3.0,7.0,3.0,7.0);
                result.setId(obj.getId());
            }

        } else if (node instanceof GenericComment) {
            GenericComment obj = (GenericComment) node;
            //System.err.printf("%s%s\n", indent(level), "GenericComment");
            
        } else if (node instanceof GenericText) {
            GenericText obj = (GenericText) node;
            //System.err.printf("%s%s\n", indent(level), "GenericText");
            
        } else if (node instanceof SVGOMDefsElement) {
            SVGOMDefsElement obj = (SVGOMDefsElement) node;
            //System.err.printf("%s%s\n", indent(level), "SVGOMDefsElement");
            
        } else if (node instanceof GenericElementNS) {
            GenericElementNS obj = (GenericElementNS) node;
            //System.err.printf("%s%s\n", indent(level), "GenericElementNS");
            
        } else if (node instanceof SVGOMMetadataElement) {
            SVGOMMetadataElement obj = (SVGOMMetadataElement) node;
            //System.err.printf("%s%s\n", indent(level), "SVGOMMetadataElement");

        // The <svg:g> element
        } else if (node instanceof SVGOMGElement) {
            SVGOMGElement obj = (SVGOMGElement) node;
            //System.err.printf("%s%s\n", indent(level), "SVGOMGElement");
            handleGroup(obj);

        // The <svg:path> element
        } else if (node instanceof SVGOMPathElement) {
            SVGOMPathElement obj = (SVGOMPathElement) node;
            //System.err.printf("%s%s\n", indent(level), "SVGOMPathElement");
            result = handlePath(obj);
            
        // The <svg:rect> element
        } else if (node instanceof SVGOMRectElement) {
            SVGOMRectElement obj = (SVGOMRectElement) node;
            //System.err.printf("%s%s\n", indent(level), "SVGOMRectElement");
            result = handleRect(obj);

        // The <svg:text> element
        } else if (node instanceof SVGOMTextElement) {
            SVGOMTextElement obj = (SVGOMTextElement) node;
            //System.err.printf("%s%s\n", indent(level), "SVGOMTextElement");
            result = handleText(obj);

        } else if (node instanceof SVGOMTSpanElement) {
            SVGOMTSpanElement obj = (SVGOMTSpanElement) node;
            //System.err.printf("%s%s\n", indent(level), "SVGOMTSpanElement");

        } else if (node instanceof SVGOMPatternElement) {
            SVGOMPatternElement obj = (SVGOMPatternElement) node;
            //System.err.printf("%s%s\n", indent(level), "SVGOMPatternElement: " + obj);
            
        } else {
            System.err.println("UNKNOWN: " + node.getClass().getName());
        }

        if (result != null) {
            // System.err.println(result);
            parentNode.getChildren().add(result);
        }


        level++;
        NodeList children = node.getChildNodes();
        for (int i = 0;  i < children.getLength();  i++) {
            org.w3c.dom.Node element = children.item(i);
            handle(element);
        }
        level--;

        parentFill = parFill;
        parentStroke = parStroke;
        parentNode = par;       // restore current parent
    }

    
    private Affine getTransform(SVGTransformable element) {
        Affine fxTrans = null;

        SVGTransformList svgTransformations = element.getTransform().getBaseVal();
        if (svgTransformations.getNumberOfItems() > 1) {
            throw new RuntimeException("More than one transformation matrix not yet supported");
        }
        if (svgTransformations.getNumberOfItems() == 1) {
            SVGTransform svgTrans = svgTransformations.getItem(0);
            SVGMatrix m = svgTrans.getMatrix();

            // SVG: matrix(0.67018323,-0.74219568,0.74219568,0.67018323,0,0)
            //         [   a    c    e  ]
            //         [   b    d    f  ]
            //         [   0    0    1  ]

            // JavaFX: [  mxx  mxy  mxz  tx  ]
            //         [  myx  myy  myz  ty  ]
            //         [  mzx  mzy  mzz  tz  ]

            fxTrans = new Affine(m.getA(), m.getC(), m.getE(), m.getB(), m.getD(), m.getF());
        }

        return fxTrans;
    }
    
    private Color getFillColor(SVGStylableElement obj) { // Shape fxObj, CSSStyleDeclaration style) {
        CSSStyleDeclaration style = obj.getStyle();

        Color result = null;
        CSSOMSVGStyleDeclaration.StyleDeclarationPaintValue val = (StyleDeclarationPaintValue) style.getPropertyCSSValue("fill");
        if (val != null && val.getPaintType() != SVGPaint.SVG_PAINTTYPE_NONE) {
            float red = val.getRed().getFloatValue(CSSPrimitiveValue.CSS_NUMBER) / 255;
            float green = val.getGreen().getFloatValue(CSSPrimitiveValue.CSS_NUMBER) / 255;
            float blue = val.getBlue().getFloatValue(CSSPrimitiveValue.CSS_NUMBER) / 255;
            CSSOMSVGStyleDeclaration.StyleDeclarationValue opacity = (StyleDeclarationValue) style.getPropertyCSSValue("fill-opacity");
            float alpha = opacity.getFloatValue(CSSPrimitiveValue.CSS_NUMBER);
            result = new Color(red, green, blue, alpha);
        }
    
        // For each styling property defined in this specification (see Property Index), 
        // there is a corresponding XML attribute (the presentation attribute) with the 
        // same name that is available on all relevant SVG elements. For example, SVG 
        // has a ‘fill’ property that defines how to paint the interior of a shape. 
        // There is a corresponding presentation attribute with the same name (i.e., ‘fill’) 
        // that can be used to specify a value for the ‘fill’ property on a given element.

        // quick google search lets us assume that style takes precedence over presentation attribute.

        if (result == null) {
            SVGStylableElement.PresentationAttributePaintValue fill =(PresentationAttributePaintValue) obj.getPresentationAttribute("fill");
            if (fill != null && fill.getPaintType() != SVGPaint.SVG_PAINTTYPE_NONE) {

                float red = fill.getRed().getFloatValue(CSSPrimitiveValue.CSS_NUMBER) / 255;
                float green = fill.getGreen().getFloatValue(CSSPrimitiveValue.CSS_NUMBER) / 255;
                float blue = fill.getBlue().getFloatValue(CSSPrimitiveValue.CSS_NUMBER) / 255;

                Object opacity = obj.getPresentationAttribute("fill-opacity");
                if (opacity != null) {
                    System.err.println("OPACITY: " + opacity.getClass());
                // float alpha = opacity.getFloatValue(CSSPrimitiveValue.CSS_NUMBER);
                //CSSOMSVGStyleDeclaration.StyleDeclarationValue opacity = (StyleDeclarationValue) style.getPropertyCSSValue("fill-opacity");
                //float alpha = opacity.getFloatValue(CSSPrimitiveValue.CSS_NUMBER);
                }

                result = new Color(red, green, blue, 1.0);
            }
        }

        if (result == null) {
            result = parentFill;  // TODO: need to search up the hierarchy ...
        }

        return result;
    }

    private Color getStrokeColor(SVGStylableElement obj) {
        CSSStyleDeclaration style = obj.getStyle();

        Color result = null;
        CSSOMSVGStyleDeclaration.StyleDeclarationPaintValue val = (StyleDeclarationPaintValue) style.getPropertyCSSValue("stroke");

        if (val != null && val.getPaintType() != SVGPaint.SVG_PAINTTYPE_NONE) {
            float red = val.getRed().getFloatValue(CSSPrimitiveValue.CSS_NUMBER) / 255;
            float green = val.getGreen().getFloatValue(CSSPrimitiveValue.CSS_NUMBER) / 255;
            float blue = val.getBlue().getFloatValue(CSSPrimitiveValue.CSS_NUMBER) / 255;

            CSSOMSVGStyleDeclaration.StyleDeclarationValue opacity = (StyleDeclarationValue) style.getPropertyCSSValue("stroke-opacity");
            float alpha = opacity.getFloatValue(CSSPrimitiveValue.CSS_NUMBER);

            result = new Color(red, green, blue, alpha);
        }

        if (result == null) {
            SVGStylableElement.PresentationAttributePaintValue stroke =(PresentationAttributePaintValue) obj.getPresentationAttribute("stroke");
            if (stroke != null && stroke.getPaintType() != SVGPaint.SVG_PAINTTYPE_NONE) {
                float red = stroke.getRed().getFloatValue(CSSPrimitiveValue.CSS_NUMBER) / 255;
                float green = stroke.getGreen().getFloatValue(CSSPrimitiveValue.CSS_NUMBER) / 255;
                float blue = stroke.getBlue().getFloatValue(CSSPrimitiveValue.CSS_NUMBER) / 255;
                //CSSOMSVGStyleDeclaration.StyleDeclarationValue opacity = (StyleDeclarationValue) style.getPropertyCSSValue("fill-opacity");
                //float alpha = opacity.getFloatValue(CSSPrimitiveValue.CSS_NUMBER);
                result = new Color(red, green, blue, 1.0);
            }            
        }
        
        if (result == null) {
            result = parentStroke;  // TODO: need to search up the hierarchy ...
        }

        return result;
    }


    private void applyStyle(Shape fxObj, SVGStylableElement obj) {

        Color fillColor = getFillColor(obj);
        fxObj.setFill(fillColor);

        Color strokeColor = getStrokeColor(obj);
        fxObj.setStroke(strokeColor);

        CSSStyleDeclaration style = obj.getStyle();
        CSSOMSVGStyleDeclaration.StyleDeclarationValue swidth = (StyleDeclarationValue) style.getPropertyCSSValue("stroke-width");
        if (swidth != null) {
            float strokeWidth = swidth.getFloatValue(CSSPrimitiveValue.CSS_NUMBER);
            fxObj.setStrokeWidth(strokeWidth);
        } else {
            SVGStylableElement.PresentationAttributeValue pwidth = (PresentationAttributeValue) obj.getPresentationAttribute("stroke-width");
            if (pwidth != null) {
                float strokeWidth = pwidth.getFloatValue(CSSPrimitiveValue.CSS_NUMBER);
                fxObj.setStrokeWidth(strokeWidth);
            }
        }

    }

    
    private void applyTextStyle(Text fxObj, SVGStylableElement obj) {
        CSSStyleDeclaration style = obj.getStyle();

        CSSOMSVGStyleDeclaration.StyleDeclarationValue val = (StyleDeclarationValue) style.getPropertyCSSValue("font-family");
        String fontFamily = val.getCssText();

        CSSOMSVGStyleDeclaration.StyleDeclarationValue val2 = (StyleDeclarationValue) style.getPropertyCSSValue("font-size");
        float fontSize = val2.getFloatValue(CSSPrimitiveValue.CSS_PX);  // https://bugs.launchpad.net/inkscape/+bug/168164

        Font font = Font.font(fontFamily, fontSize);
        fxObj.setFont(font);

/*        font-style:normal;
        font-variant:normal;
        font-weight:normal;
        font-stretch:normal;
*/
    }

    private Node handleText(SVGOMTextElement obj) {
        // Get attributes from SVG node
        String text = obj.getTextContent();
        float xpos = obj.getX().getBaseVal().getItem(0).getValue();
        float ypos = obj.getY().getBaseVal().getItem(0).getValue();

        // Create JavaFX text object
        Text fxObj = new Text(xpos, ypos, text);
        fxObj.setId(obj.getId());

        Affine transformation = getTransform(obj);
        if (transformation != null) {
            fxObj.getTransforms().add(transformation);
        }

        applyTextStyle(fxObj, obj);

        return fxObj;
    }

    private Node handleRect(SVGOMRectElement obj) {
        // Get attributes from SVG node
        float xpos = obj.getX().getBaseVal().getValue();
        float ypos = obj.getY().getBaseVal().getValue();
        float width = obj.getWidth().getBaseVal().getValue();
        float height = obj.getHeight().getBaseVal().getValue();
        float rX = obj.getRx().getBaseVal().getValue();
        float rY = obj.getRy().getBaseVal().getValue();

        // Create JavaFX Rectangle object
        Rectangle fxObj = new Rectangle(xpos, ypos, width, height);
        fxObj.setId(obj.getId());
        fxObj.setArcWidth(2*rX);
        fxObj.setArcHeight(2*rY);

        Affine transformation = getTransform(obj);
        if (transformation != null) {
            fxObj.getTransforms().add(transformation);
        }

        applyStyle(fxObj, obj);
        
        return fxObj;
    }

    
    private Node handlePath(SVGOMPathElement element) {
        // Get attributes from SVG node
        String path = element.getAttribute("d");

        // Create JavaFX SVGPath object
        SVGPath fxObj = new SVGPath();
        fxObj.setContent(path);
        fxObj.setId(element.getId());

        Affine transformation = getTransform(element);
        if (transformation != null) {
            fxObj.getTransforms().add(transformation);
        }

        applyStyle(fxObj, element);

        //fxObj.setStroke(Color.VIOLET);
        return fxObj;
    }


    /**
     * Alternative handling of an SVG path.
     * Creates distinct JavaFX Shapes for each path segment.
     * 
     * @param obj
     */
    private Node handlePath2(SVGOMPathElement element) {
        
        Group g = new Group();
        g.setId(element.getId());

        double xpos = 0.0;
        double ypos = 0.0;

        //CSSStyleDeclaration style = obj.getStyle();
        SVGOMAnimatedPathData.BaseSVGPathSegList list = (BaseSVGPathSegList) element.getPathSegList();
        System.err.printf("%s%s\n", indent(level), "PATH: " + list.getNumberOfItems());
        for (int i = 0;  i < list.getNumberOfItems();  i++) {
            SVGPathSegItem item = (SVGPathSegItem) list.getItem(i);
            Shape fxObj = null;

            switch(item.getPathSegType()) { 
                case SVGPathSegItem.PATHSEG_UNKNOWN : 
                    System.err.printf("%s PATHSEG_UNKNOWN\n", indent(level+2));
                    break;
                
                case SVGPathSegItem.PATHSEG_CLOSEPATH : 
                    System.err.printf("%s PATHSEG_CLOSEPATH\n", indent(level+2));
                    break;

                case SVGPathSegItem.PATHSEG_MOVETO_ABS : 
                    xpos = item.getX();
                    ypos = item.getY();
                    System.err.printf("%s moveto(%f/%f)\n", indent(level+2), xpos, ypos);
                    break;
                
                case SVGPathSegItem.PATHSEG_MOVETO_REL :
                    xpos += item.getX();
                    ypos += item.getY();
                    System.err.printf("%s moveto(%f/%f)\n", indent(level+2), xpos, ypos);
                    break;

                case SVGPathSegItem.PATHSEG_LINETO_ABS  : { 
                        double x2 = item.getX();
                        double y2 = item.getY();
                        
                        System.err.printf("%s lineto(%f/%f)\n", indent(level+2), x2, y2);
                        fxObj = new Line(xpos, ypos, x2, y2);
                        
                        xpos = x2;
                        ypos = y2;
                    }
                    break;

                case SVGPathSegItem.PATHSEG_LINETO_REL  : {
                        double x2 = xpos + item.getX();
                        double y2 = ypos + item.getY();
                        
                        System.err.printf("%s lineto(%f/%f)\n", indent(level+2), x2, y2);
                        fxObj = new Line(xpos, ypos, x2, y2);
                        
                        xpos = x2;
                        ypos = y2;
                    }
                    break;

                case SVGPathSegItem.PATHSEG_CURVETO_CUBIC_ABS  : {
                        double endX = xpos + item.getX();
                        double endY = ypos + item.getY();
                        
                        System.err.printf("%s cubicCurve(%f/%f, %f/%f)\n", indent(level+2), xpos, ypos, endX, endY);
                        fxObj = new CubicCurve(xpos, ypos, item.getX1(), item.getY1(), item.getX2(), item.getY2(), endX, endY);

                        xpos = endX;
                        ypos = endY;
                    }
                    break;

                case SVGPathSegItem.PATHSEG_CURVETO_CUBIC_REL : { 
                        double endX = xpos + item.getX();
                        double endY = ypos + item.getY();
                        double x1 = xpos + item.getX1();
                        double y1 = ypos + item.getY1();
                        double x2 = xpos + item.getX2();
                        double y2 = ypos + item.getY2();
                        
                        System.err.printf("%s cubicCurve(%f/%f, %f/%f)\n", indent(level+2), xpos, ypos, endX, endY);
                        fxObj = new CubicCurve(xpos, ypos, x1, y1, x2, y2, endX, endY);

                        xpos = endX;
                        ypos = endY;
                    }
                    break;

                default:
                    System.err.printf("%s UNKNOWN\n", indent(level+2));
                    break;
            }

            if (fxObj != null) {
                
                System.err.printf("%s\n", element);
                applyStyle(fxObj, element);

                g.getChildren().add(fxObj);
            }
        }

        return g;
    }


    private void handleGroup(SVGOMGElement obj) {
        Group fxObj = new Group();
        fxObj.setId(obj.getId());

        Affine transformation = getTransform(obj);
        if (transformation != null) {
            fxObj.getTransforms().add(transformation);
        }

        // TODO: Can a group inherit its presentation properties to its children??
        //applyStyle(fxObj, obj);
        //fxObj.setStyle("-fx-background-color: blue; -fx-stroke-color: yellow;");
        parentStroke = getStrokeColor(obj);
        parentFill = getFillColor(obj);

        parentNode.getChildren().add(fxObj);
        parentNode = fxObj;
    }


    /**
     * @param The name of the SVG file to load.
     * @return An XML document with the loaded SVG file.
     */
    private Document loadSvgDocument(String fileName) {
        String parser = XMLResourceDescriptor.getXMLParserClassName();
        SAXSVGDocumentFactory f = new SAXSVGDocumentFactory(parser);
        try {
            String uri = new File(fileName).toURL().toString();
            SVGOMDocument doc = (SVGOMDocument) f.createDocument(uri);
            SVGDOMImplementation impl = (SVGDOMImplementation) ((SVGOMDocument) doc).getImplementation();
            CSSEngine eng = impl.createCSSEngine(doc, this);

            doc.setCSSEngine(eng);

            return doc;
        } catch (MalformedURLException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return null;
    }


    /**
     * @param doc An XML document with an SVG file.
     * @return The rendered image for the SVG document.
     */
    public Image getImage(Document doc) {
        Image result = null;

        try {
            BufferedImageTranscoder transcoder = new BufferedImageTranscoder();
            TranscoderInput transIn = new TranscoderInput(doc);
            transcoder.transcode(transIn, null);
            result = SwingFXUtils.toFXImage(transcoder.getBufferedImage(), null);
        } catch (TranscoderException e) {
            e.printStackTrace();
        }

        return result;
    }


    @Override
    public Value getSystemColor(String ident) {
        // TODO Auto-generated method stub
        return null;
    }


    @Override
    public Value getDefaultFontFamily() {
        // TODO Auto-generated method stub
        return null;
    }


    @Override
    public float getLighterFontWeight(float f) {
        // TODO Auto-generated method stub
        return 0;
    }


    @Override
    public float getBolderFontWeight(float f) {
        // TODO Auto-generated method stub
        return 0;
    }


    @Override
    public float getPixelUnitToMillimeter() {
        // TODO: check!
        int dpi = Screen.getMainScreen().getResolutionX();  // what about Y?
        return 25.4f / dpi;
    }


    @Override
    public float getPixelToMillimeter() {
        // TODO Auto-generated method stub
        return 0;
    }


    @Override
    public float getMediumFontSize() {
        // TODO Auto-generated method stub
        return 0;
    }


    @Override
    public float getBlockWidth(Element elt) {
        // TODO Auto-generated method stub
        return 0;
    }


    @Override
    public float getBlockHeight(Element elt) {
        // TODO Auto-generated method stub
        return 0;
    }


    @Override
    public void checkLoadExternalResource(ParsedURL resourceURL,
            ParsedURL docURL) throws SecurityException {
        // TODO Auto-generated method stub
        
    }


    @Override
    public boolean isDynamic() {
        // TODO Auto-generated method stub
        return false;
    }


    @Override
    public boolean isInteractive() {
        // TODO Auto-generated method stub
        return false;
    }


    @Override
    public CSSEngine getCSSEngineForElement(Element e) {
        // TODO Auto-generated method stub
        return null;
    }

}
