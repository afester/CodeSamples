import 'dart:html';

void main() {
  CanvasElement editorArea = query('#editor-area');
  CanvasRenderingContext2D context = editorArea.getContext('2d');
  //context.scale(2,2);
//  context.fillStyle = 'red';
//  context.borderStyle = 'black';
//  context.fillRect(30, 30, 50, 50);
//  context.rect(50, 50, 50, 50);
  context.font = "10pt arial,sans-serif";
//  context.strokeText("Hello World", 100, 100);
  context.fillText("Lorem <b>ipsum</b> dolor sit amet, consectetur adipiscing elit. Integer pellentesque posuere facilisis. Aenean eu dolor ante, ac blandit nisl. Nullam iaculis sollicitudin nisl, sed pretium diam tristique eget. Cras quam nisi, tristique ac ultricies eget, fermentum nec felis. Duis elementum ultrices metus ac pharetra. Phasellus molestie imperdiet dolor ut facilisis. Nam accumsan ullamcorper velit ac auctor. Nam quis velit magna. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Fusce eget libero magna. Praesent auctor, diam egestas pharetra euismod, lectus metus condimentum eros, vitae faucibus urna lorem ac tortor. Vivamus scelerisque urna in purus sollicitudin ultrices. Curabitur adipiscing posuere magna non varius. Cras varius, orci pharetra aliquam scelerisque, lorem erat feugiat augue, in suscipit est diam placerat lectus.",
                    0, 10);

  var loadButton = query('#loadButton');
  loadButton.onClick.listen( (event) {
    String contents = """  
    <h1>Section 1</h1>
    <p class="standard">Lorem <b>ipsum</b> dolor sit amet, consectetur adipiscing elit. Integer pellentesque posuere facilisis. Aenean eu dolor ante, ac blandit nisl. Nullam iaculis sollicitudin nisl, sed pretium diam tristique eget. Cras quam nisi, tristique ac ultricies eget, fermentum nec felis. Duis elementum ultrices metus ac pharetra. Phasellus molestie imperdiet dolor ut facilisis. Nam accumsan ullamcorper velit ac auctor. Nam quis velit magna. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Fusce eget libero magna. Praesent auctor, diam egestas pharetra euismod, lectus metus condimentum eros, vitae faucibus urna lorem ac tortor. Vivamus scelerisque urna in purus sollicitudin ultrices. Curabitur adipiscing posuere magna non varius. Cras varius, orci pharetra aliquam scelerisque, lorem erat feugiat augue, in suscipit est diam placerat lectus.</p>
    <p class="code">Paragraph in code format</p>""";

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
    window.document.execCommand("insertBrOnReturn", false, "False");
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
}
