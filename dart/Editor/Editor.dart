/**
 * See http://dev.opera.com/articles/view/rich-html-editing-in-the-browser-part-1/
 * See http://blog.whatwg.org/the-road-to-html-5-contenteditable
 */

import 'dart:html';

void main() {
  DivElement editorArea = query('#editor-area');
  editorArea.contentEditable = "true";  // HTMl 5!!

  /**
  https://bugzilla.mozilla.org/show_bug.cgi?id=503838
  "The editing spec currently requires that empty blocks be filled in with a <br>, 
   which is then removed when it's no longer necessary."
  */
  editorArea.onKeyDown.listen( (KeyboardEvent event) {
    if (event.keyCode == 13) {
      Selection sel = window.getSelection();
      Range r = sel.getRangeAt(0);
      Element el = new ParagraphElement();
//      el.innerHTML = "Hello World";
      log(el.innerHtml);
      DocumentFragment frag = document.createDocumentFragment();
      var node, lastNode;
      node = el.$dom_firstChild;
      while ( node != null ) {
          lastNode = frag.children.add(node);
          node = el.$dom_firstChild;
      }

      r.insertNode(frag);

/*      
      Node node = r.startContainer;
//      log(node.hasChildNodes().toString());
      // Node p = new Text("Hello World");
      Node p = new ParagraphElement();
      node.$dom_appendChild(p);

      var startPos = myField.selectionStart;
      var endPos = myField.selectionEnd;
      myField.value = myField.value.substring(0, startPos)
      + myValue
      + myField.value.substring(endPos, myField.value.length);
      } else {
      myField.value += myValue;
      }
*/
//      log(node.toString());
//      node = node.nextNode;
//      log(node.hasChildNodes() ? "true" : "false");// .toString());
      //editorArea. . .selection.createRange().pasteHTML("<br/>")
//      ParagraphElement pNode = new ParagraphElement();
//      Range range = window.getSelection().getRangeAt(0);
//      range.insertNode(pNode);
      event.preventDefault();
    }
  });

  var loadButton = query('#loadButton');
  loadButton.onClick.listen( (event) {
    String contents = """  
    <h1>Section 1</h1>
    <p class="standard">Lorem <b>ipsum</b> dolor sit amet</p>
    <p class="code">Paragraph in code format</p>""";
/*    String contents = """  
    <h1>Section 1</h1>
    <p class="standard">Lorem <b>ipsum</b> dolor sit amet, consectetur adipiscing elit. Integer pellentesque posuere facilisis. Aenean eu dolor ante, ac blandit nisl. Nullam iaculis sollicitudin nisl, sed pretium diam tristique eget. Cras quam nisi, tristique ac ultricies eget, fermentum nec felis. Duis elementum ultrices metus ac pharetra. Phasellus molestie imperdiet dolor ut facilisis. Nam accumsan ullamcorper velit ac auctor. Nam quis velit magna. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Fusce eget libero magna. Praesent auctor, diam egestas pharetra euismod, lectus metus condimentum eros, vitae faucibus urna lorem ac tortor. Vivamus scelerisque urna in purus sollicitudin ultrices. Curabitur adipiscing posuere magna non varius. Cras varius, orci pharetra aliquam scelerisque, lorem erat feugiat augue, in suscipit est diam placerat lectus.</p>
    <p class="code">Paragraph in code format</p>""";
*/
    var editorPane = query('#editor-area');
    editorPane.innerHTML = contents;
  });

  var saveButton = query('#saveButton');
  saveButton.onClick.listen( (event) {
    var editorPane = query('#editor-area');
    var resultPane = query('#result');
    resultPane.text = "<html>${editorPane.innerHTML}\n</html>";
  });
  
  // add action listeners for the command buttons
  query('#alignLeftButton').onClick.listen( (event) {
    window.document.execCommand("justifyleft", false, "");
  });

  query('#alignBlockButton').onClick.listen( (event) {
    window.document.execCommand("justifyfull", false, "");
  });

  query('#alignRightButton').onClick.listen( (event) {
    window.document.execCommand("justifyright", false, "");
  });

  query('#boldButton').onClick.listen( (event) {
    window.document.execCommand("bold", false, "");
  });

  query('#italicButton').onClick.listen( (event) {
    window.document.execCommand("italic", false, "");
  });

  query('#underlineButton').onClick.listen( (event) {
    window.document.execCommand("underline", false, "");
  });

  query('#orderedListButton').onClick.listen( (event) {
    window.document.execCommand("insertorderedlist", false, "");
  });

  query('#unorderedListButton').onClick.listen( (event) {
    window.document.execCommand("insertunorderedlist", false, "");
  });

  query('#indentButton').onClick.listen( (event) {
    window.document.execCommand("indent", false, "");
  });
  
  query('#outdentButton').onClick.listen( (event) {
    window.document.execCommand("outdent", false, "");
  });
  
  query('#undoButton').onClick.listen( (event) {
    window.document.execCommand("undo", false, "");
  });
  
  query('#redoButton').onClick.listen( (event) {
    window.document.execCommand("redo", false, "");
  });

  // toggle pilcrow symbol on paragraph ends
  bool pilcrow = true;
//  CSSStyleRule pRule = null;
//  CSSStyleRule h1Rule = null;

  query('#pilcrowButton').onClick.listen( (event) {
    window.close();

    CssStyleSheet css = document.styleSheets[0];
    
    if (pilcrow) {
      List<CssRule> rules = css.cssRules;
  
      String resultText = "";
      for (int i = rules.length  - 1;  i >= 0;  i--) {
        CssStyleRule rule = rules.elementAt(i);
        if (rule.selectorText == "p::after") {
//          pRule = rule;
          css.removeRule(i);
        } else if (rule.selectorText == "h1::after") {
//          h1Rule = rule;
          css.removeRule(i);
        }
      }
      pilcrow = false;
    } else {
      pilcrow = true;
      css.addRule("p::after",  "content: \"¶\"");
      css.addRule("h1::after", "content: \"¶\"");

//      var resultPane = query('#result');
//      resultPane.text = pRule.style.toString();
    }
  });
  
  
  
}


void log(String msg) {
  var resultPane = query('#result');
  resultPane.text = msg;
}
