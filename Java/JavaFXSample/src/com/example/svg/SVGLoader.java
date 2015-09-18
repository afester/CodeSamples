package com.example.svg;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;

import org.apache.batik.anim.dom.SAXSVGDocumentFactory;
import org.apache.batik.anim.dom.SVGDOMImplementation;
import org.apache.batik.anim.dom.SVGOMDefsElement;
import org.apache.batik.anim.dom.SVGOMDocument;
import org.apache.batik.anim.dom.SVGOMGElement;
import org.apache.batik.anim.dom.SVGOMMetadataElement;
import org.apache.batik.anim.dom.SVGOMPathElement;
import org.apache.batik.anim.dom.SVGOMRectElement;
import org.apache.batik.anim.dom.SVGOMSVGElement;
import org.apache.batik.anim.dom.SVGOMTSpanElement;
import org.apache.batik.anim.dom.SVGOMTextElement;
import org.apache.batik.css.engine.CSSContext;
import org.apache.batik.css.engine.CSSEngine;
import org.apache.batik.css.engine.SVGCSSEngine;
import org.apache.batik.css.engine.value.Value;
import org.apache.batik.css.parser.Parser;
import org.apache.batik.dom.GenericComment;
import org.apache.batik.dom.GenericElementNS;
import org.apache.batik.dom.GenericText;
import org.apache.batik.transcoder.TranscoderException;
import org.apache.batik.transcoder.TranscoderInput;
import org.apache.batik.transcoder.TranscoderOutput;
import org.apache.batik.transcoder.image.ImageTranscoder;
import org.apache.batik.util.ParsedURL;
import org.apache.batik.util.XMLResourceDescriptor;
import org.w3c.css.sac.CSSException;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
import org.w3c.dom.css.CSSStyleDeclaration;
import org.w3c.dom.css.CSSValue;

import javafx.embed.swing.SwingFXUtils;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;
import javafx.scene.paint.Paint;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Text;

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

    
    
    public SVGLoader() {
    }
    

    private Pane parentNode;
    private Pane result; 

    /**
     * @param fileName The name of the SVG file to load.
     * @return A JavaFX node representing the SVG file.
     */
    public Node loadSvg(String fileName) {
        // note: uses the DOM approach.
        // probably a SAX based approach would be better from a performance perspective.
        SVGOMDocument doc = (SVGOMDocument) loadSvgDocument(fileName);

        result = new Pane();
        parentNode = result;
        handle(doc);
        return result;
    }

    
    private String indent(int level) {
        return "                                    ".substring(0, level*2);
    }

    
    private int level = 0;
    
    private void handle(org.w3c.dom.Node node) {
        if (node instanceof SVGOMDocument) {
            SVGOMDocument obj = (SVGOMDocument) node;
            //System.err.printf("%s%s\n", indent(level), "SVGOMDocument");

        // The <svg> tag
        } else if (node instanceof SVGOMSVGElement) {
            SVGOMSVGElement obj = (SVGOMSVGElement) node;
            //System.err.printf("%s%s\n", indent(level), "SVGOMSVGElement");

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
            handlePath(obj);
            
        // The <svg:rect> element
        } else if (node instanceof SVGOMRectElement) {
            SVGOMRectElement obj = (SVGOMRectElement) node;
            //System.err.printf("%s%s\n", indent(level), "SVGOMRectElement");
            handleRect(obj);

        // The <svg:text> element
        } else if (node instanceof SVGOMTextElement) {
            SVGOMTextElement obj = (SVGOMTextElement) node;
            //System.err.printf("%s%s\n", indent(level), "SVGOMTextElement");
            handleText(obj);

        } else if (node instanceof SVGOMTSpanElement) {
            SVGOMTSpanElement obj = (SVGOMTSpanElement) node;
            //System.err.printf("%s%s\n", indent(level), "SVGOMTSpanElement");

        } else {
            System.err.println("UNKNOWN!");
        }

        level++;
        Pane par = parentNode;  // save current parent
        NodeList children = node.getChildNodes();
        for (int i = 0;  i < children.getLength();  i++) {
            org.w3c.dom.Node element = children.item(i);
            handle(element);
        }
        parentNode = par;       // restore current parent
        level--;
    }


    private void handleText(SVGOMTextElement obj) {
        String text = obj.getTextContent();
        float xpos = Float.parseFloat(obj.getAttribute("x"));
        float ypos = Float.parseFloat(obj.getAttribute("y"));
        System.err.printf("%s%s\n", indent(level), "TEXT: " + text + "/" + xpos + "/" + ypos + "/" + obj.getTransform().getBaseVal().getNumberOfItems());

        parentNode.getChildren().add(new Text(xpos, ypos, text));
    }

    int x = 0;
    private void handleRect(SVGOMRectElement obj) {
        float xpos = Float.parseFloat(obj.getAttribute("x"));
        float ypos = Float.parseFloat(obj.getAttribute("y"));
        float width = Float.parseFloat(obj.getAttribute("width"));
        float height = Float.parseFloat(obj.getAttribute("height"));
        System.err.printf("%sRECT(%f, %f, %f, %f)\n", indent(level), xpos, ypos, width, height);

        CSSStyleDeclaration style = obj.getStyle();
        //CSSStyleDeclaration style = obj.getOverrideStyle();
        //CSSValue fill = style.getPropertyCSSValue("fill");
        //CSSValue fill = obj.getPresentationAttribute("fill");
        System.err.println("FILL: " + style.getPropertyCSSValue("fill").getValue()); // .getCssValueType());// .getPropertyValue("fill"));

        Paint paint = null;
        switch(x) {
        case 0 : paint = new Color(1.0, 0.0, 0.0, 0.5);break;
        case 1 : paint = new Color(0.0, 1.0, 0.0, 0.5);break;
        case 2 : paint = new Color(0.0, 0.0, 1.0, 0.5);break;
        default :paint = new Color(0.0, 1.0, 1.0, 0.5);break;
        }
        x++;
        Rectangle fxObj = new Rectangle(xpos, ypos, width, height);
        fxObj.setFill(paint);
        fxObj.setId(obj.getId());

        parentNode.getChildren().add(fxObj);
    }

    private void handlePath(SVGOMPathElement obj) {
        System.err.printf("%s%s\n", indent(level), "PATH: "); 
    }

    private void handleGroup(SVGOMGElement obj) {
        System.err.printf("%s%s\n", indent(level), "GROUP: ");
        Pane group = new Pane();
        parentNode.getChildren().add(group);
        parentNode = group;
    }


    /**
     * @param The name of the SVG file to load.
     * @return An XML document with the loaded SVG file.
     */
    public Document loadSvgDocument(String fileName) {
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
        // TODO Auto-generated method stub
        return 0;
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
