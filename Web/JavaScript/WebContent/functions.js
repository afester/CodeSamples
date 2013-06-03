/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

// the global function graph view object
var FGV = {};

/**
 * Convert a formula as entered by the user into an JavaScript expression 
 * TODO: Use a real parser.
 * 
 * @param formula The formula to convert (e.g. "sin(x)"
 * @returns The corresponding JavaScript expression (e.g. "Math.sin(x)"
 */
function convertFormula(formula) {
   result = formula;

   result = result.replace(/sin/g, "Math.sin");
   result = result.replace(/cos/g, "Math.cos");
   result = result.replace(/tan/g, "Math.tan");

   result = result.replace(/pow/g, "Math.pow");
   result = result.replace(/log/g, "Math.log");
   result = result.replace(/exp/g, "Math.exp");

   result = result.replace(/sqrt/g, "Math.sqrt");

   return result;
}


/**
 * Updates the graph view parameters from the X axis range entered by the user.
 */
function updateValues() {
   // get the x axis range as entered by the user
   FGV.startX = parseFloat(FGV.fromX.value);
   FGV.stopX = parseFloat(FGV.toX.value);
   var xRange = FGV.stopX - FGV.startX;

   // calculate graph parameters with the assumption that there is enough space
   // for the y axis scale
   FGV.paddingLeft = 0;

   // auto-calculate the Y axis range
   FGV.startY = -(xRange * FGV.canvas.height) / (2 * FGV.canvas.width);
   FGV.stopY = -FGV.startY;

   // calculate the scale factor
   FGV.scalePerPixel = (FGV.stopX - FGV.startX) / FGV.canvas.width;

   // calculate the position of the origin point
   FGV.x0pos = -FGV.startX / FGV.scalePerPixel;
   FGV.y0pos = -FGV.startY / FGV.scalePerPixel;

   // recalculate graph parameters with a left padding if there is not enough
   // space for the y axis scale
   if (FGV.x0pos < 30) {
      // calculate graph parameters and reserve space for the y axis scale
      FGV.paddingLeft = 30 - FGV.x0pos;

      // auto-calculate the Y axis range
      FGV.startY = -(xRange * FGV.canvas.height) / (2 * (FGV.canvas.width - FGV.paddingLeft));
      FGV.stopY = -FGV.startY;

      // calculate the scale factor
      FGV.scalePerPixel = (FGV.stopX - FGV.startX) / (FGV.canvas.width - FGV.paddingLeft);

      // calculate the position of the origin point
      FGV.x0pos = -FGV.startX / FGV.scalePerPixel + FGV.paddingLeft;
      FGV.y0pos = -FGV.startY / FGV.scalePerPixel;
   }

   // update Y range scale as shown on the UI
   FGV.fromY.value = Math.round(FGV.startY * 100) / 100;
   FGV.toY.value = Math.round(FGV.stopY * 100) / 100;

   // calculate the distance for each tick
   FGV.tick = 20 * FGV.scalePerPixel;
   if (FGV.tick < 0.1) {
      FGV.tick = 0.1;
   } else if (FGV.tick < 0.5) {
      FGV.tick = 0.5;
   } else if (FGV.tick < 1.0) {
      FGV.tick = 1.0;
   }
}


/**
 * Draws a tick on the Y axis.
 */
function drawYTick(y) {
   var ypos = FGV.y0pos - y / FGV.scalePerPixel;
   if (ypos > 20) { // do not draw a tick nearby the end of the axis!
      FGV.context.moveTo(FGV.x0pos - 5, ypos);
      FGV.context.lineTo(FGV.x0pos + 5, ypos);

      var value = Math.round(y * 10) / 10;
      var textWidth = FGV.context.measureText(value).width;
      FGV.context.fillText(value, FGV.x0pos - textWidth - 7, ypos + 3);
   }
}


/**
 * Draws a tick on the X axis.
 */
function drawXTick(x) {
   var xpos = FGV.x0pos + x / FGV.scalePerPixel;
   FGV.context.moveTo(xpos, FGV.y0pos - 5);
   FGV.context.lineTo(xpos, FGV.y0pos + 5);

   var value = Math.round(x * 10) / 10;
   var textWidth = FGV.context.measureText(value).width;
   FGV.context.fillText(value, xpos - textWidth / 2, FGV.y0pos + 15);
}


/**
 * Draws the axes including the ticks.
 */
function drawAxis() {
   // set the axis visual properties
   FGV.context.beginPath();
   FGV.context.strokeStyle = "black";
   FGV.context.lineWidth = 1;
   FGV.context.setLineDash([]);

   // draw the axis lines

   // Y axis
   FGV.context.moveTo(FGV.x0pos, FGV.canvas.height);
   FGV.context.lineTo(FGV.x0pos, 0);

   // X axis
   FGV.context.moveTo(FGV.paddingLeft, FGV.y0pos);
   FGV.context.lineTo(FGV.canvas.width, FGV.y0pos);

   // draw the y scale
   for ( var y = -FGV.tick; y > FGV.startY; y -= FGV.tick) { // negative axis
      drawYTick(y);
   }
   for ( var y = FGV.tick; y < FGV.stopY; y += FGV.tick) { // positive axis
      drawYTick(y);
   }

   // draw the x scale
   for ( var x = -FGV.tick; x > FGV.startX; x -= FGV.tick) { // negative axis
      drawXTick(x);
   }
   for ( var x = FGV.tick; x < FGV.stopX; x += FGV.tick) { // positive axis
      drawXTick(x);
   }

   FGV.context.stroke();

   // Y axis arrow
   FGV.context.moveTo(FGV.x0pos, 0);
   FGV.context.lineTo(FGV.x0pos - 5, 10);
   FGV.context.lineTo(FGV.x0pos + 5, 10);
   FGV.context.lineTo(FGV.x0pos, 0);
   FGV.context.strokeText("Y", FGV.x0pos + FGV.paddingLeft - 15, 10);

   // X axis arrow
   FGV.context.moveTo(FGV.canvas.width, FGV.y0pos);
   FGV.context.lineTo(FGV.canvas.width - 10, FGV.y0pos - 5);
   FGV.context.lineTo(FGV.canvas.width - 10, FGV.y0pos + 5);
   FGV.context.lineTo(FGV.canvas.width, FGV.y0pos);
   FGV.context.strokeText("X", FGV.canvas.width - 10, FGV.y0pos + 20);

   FGV.context.fill();
}


function getElementByXpath(path) {
   return document.evaluate(path, document, null, XPathResult.FIRST_ORDERED_NODE_TYPE, null).singleNodeValue;
}


function setError(theGraph, theException) {
   var errorText = getElementByXpath('//*[@id="_legendTable"]/tr['+ (theGraph.index+1) +']/td/font[2]/text()');
   errorText.data = '\u00a0\u00a0\u00a0Error: ' + theException;

/*   
   var table = document.getElementById("_legendTable");
   if (table) {
      var tableCell = table.childNodes[theGraph.index].childNodes[0]; // <td> node
      var errorText = tableCell.childNodes[2].childNodes[0];          // <font> node
      errorText.data = '\u00a0\u00a0\u00a0Error: ' + theException;
   }*/
}


/**
 * Draws a specific graph.
 * 
 * @param theGraph  The graph object to draw.
 */
function drawGraph(theGraph) {
   FGV.context.beginPath();
   FGV.context.strokeStyle = theGraph.color;
   FGV.context.lineWidth = 1;
   FGV.context.setLineDash([]);

   // draw the graph
   var x = FGV.startX;
   var y = 0;
   try {
      y = eval(theGraph.formula);
   } catch(e) {
      setError(theGraph, e);
      return;
   }
   if (y == Infinity) {
      y = 1000;
   }

   var xpos = FGV.x0pos + x / FGV.scalePerPixel;
   var ypos = FGV.y0pos - y / FGV.scalePerPixel;
   FGV.context.moveTo(xpos, ypos);

   x += FGV.scalePerPixel;
   for (; x < FGV.stopX; x += FGV.scalePerPixel) {
      var y = eval(theGraph.formula);

      var xpos = FGV.x0pos + x / FGV.scalePerPixel;
      var ypos = FGV.y0pos - y / FGV.scalePerPixel;
      FGV.context.lineTo(xpos, ypos);
   }

   FGV.context.stroke();
}


/**
 * Adds an entry to the graph legend.
 * 
 * @param graph
 *           The graph object which contains the color, the text and the
 *           JavaScript function of the graph
 */
function addLegendEntry(graph) {
   // Get the legend table and add it if it does not exist yet
   var table = document.getElementById("_legendTable");
   if (!table) {
      table = document.createElement('table');
      table.setAttribute('id', '_legendTable');
      FGV.legendDiv.appendChild(table);
   }

   // Append a new table row as the legend entry
   var tr = document.createElement('tr');
   var td = document.createElement('td');
   tr.appendChild(td);

   // create a text node containing the function name, colored in the graph's
   // color
   // <font color="red">f<sub>1</sub>(x) = </font>
   var font = document.createElement('font');
   font.setAttribute('color', graph.color);

   // "f"
   font.appendChild(document.createTextNode('f'));

   // <sub>n</sub>
   var subScript = document.createElement('sub');
   var fnIndex = document.createTextNode("" + graph.index);
   subScript.appendChild(fnIndex);
   font.appendChild(subScript);

   // "(x) ="
   font.appendChild(document.createTextNode('(x) = '));

   // create a text node containing the actual function
   var fnTextNode = document.createTextNode(graph.text);

   // create a text node which can display an error message
   var errorFont = document.createElement('font');
   errorFont.setAttribute('color', 'red');

   var errorText = document.createTextNode("");
   errorFont.appendChild(errorText);

   td.appendChild(font);
   td.appendChild(fnTextNode);
   td.appendChild(errorFont);

   table.appendChild(tr);
}


/**
 * Adds a graph to the graph array and to the legend, but does not draw the
 * graph yet.
 * 
 * @param formula The function to add.
 * @param color The color for the function graph.
 */
function addGraph(formula, color) {

   var graph = {
      "index" : FGV.graphs.length,
      "text" : formula,
      "formula" : convertFormula(formula),
      "color" : color
   };

   FGV.graphs.push(graph);
   addLegendEntry(graph);
}


/**
 * Redraws the complete canvas.
 */
function renderScene() {
   FGV.context.clearRect(0, 0, FGV.canvas.width, FGV.canvas.height);

   drawAxis();

   for ( var i = 0; i < FGV.graphs.length; i++) {
      drawGraph(FGV.graphs[i]);
   }
}


/**
 * Event function for the canvas mouse move events. 
 * Updates the crosshair coordinates when the mouse moves.
 * 
 * @param evt The event object
 */
function onMouseMove(evt) {
   var mouseXpos = evt.clientX - FGV.canvas.offsetLeft;
   var mouseYpos = evt.clientY - FGV.canvas.offsetTop;

   renderScene();

   // set visual properties for the cross
   FGV.context.beginPath();
   FGV.context.strokeStyle = "red";
   FGV.context.lineWidth = 0.5;
   FGV.context.setLineDash([ 8, 2 ]);

   // draw the cross
   FGV.context.moveTo(mouseXpos, 0);
   FGV.context.lineTo(mouseXpos, FGV.canvas.height);
   FGV.context.moveTo(0, mouseYpos);
   FGV.context.lineTo(FGV.canvas.width, mouseYpos);
   FGV.context.stroke();

   // update position values
   FGV.curXposNode.value = Math.round(((mouseXpos - FGV.x0pos) * FGV.scalePerPixel) * 100) / 100;
   FGV.curYposNode.value = Math.round(((FGV.y0pos - mouseYpos) * FGV.scalePerPixel) * 100) / 100;
}


/**
 * Event function for the canvas mouse out events. 
 * Whenever the mouse leaves the canvas, the crosshair is removed and 
 * the crosshair coordinates are set to "n/a".
 *
 * @param evt  The event object
 */
function onMouseOut(evt) {
   FGV.curXposNode.value = 'n/a';
   FGV.curYposNode.value = 'n/a';
   renderScene();
}


/**
 * Event function for the "onload" event. This function is called when the page
 * has been loaded.
 */
function initialize() {
   // create global variables for the elements we want to access.
   // Not all browsers support the global element variables, especially not
   // in HTML5 mode.
   FGV.formula = document.getElementById("_formula");
   FGV.graphColor = document.getElementById("_graphColor");
   FGV.fromX = document.getElementById("_fromX");
   FGV.toX = document.getElementById("_toX");
   FGV.fromY = document.getElementById("_fromY");
   FGV.toY = document.getElementById("_toY");
   FGV.canvas = document.getElementById("_canvas");
   FGV.curXposNode = document.getElementById("_curXpos");
   FGV.curYposNode = document.getElementById("_curYpos");
   FGV.legendDiv = document.getElementById("_legend");

   FGV.canvas.onmousemove = onMouseMove;
   FGV.canvas.onmouseout = onMouseOut;
   FGV.context = FGV.canvas.getContext('2d');
   FGV.context.font = '8pt arial,sans-serif';

   // Not all browsers support setLineDash!
   // http://www.rgraph.net/blog/2013/january/html5-canvas-dashed-lines.html
   if (!FGV.context.setLineDash) {
      FGV.context.setLineDash = function() {
      };
   }

   FGV.graphs = [];

   // Some sample graphs, especially for debugging purposes
   addGraph("2*sin(x)", "red");
   addGraph("log(X)", "blue");
   addGraph("exp(x)", "green");
   addGraph("x + 3*pow(x, 2) + pow(x, 3) - 1", "maroon");

   updateValues();
   renderScene();
}


/**
 * Action function for the "Add" button.
 */
function addAction() {
   addGraph(FGV.formula.value, FGV.graphColor.value);
   renderScene();
}


/**
 * Action function for the "Apply" button on the X axis.
 */
function applyAction() {
   updateValues();
   renderScene();
}


/**
 * Action function for the "Clear all" button.
 */
function clearAction() {
   // remove the legend table
   var table = document.getElementById("_legendTable");
   if (table) {
      table.parentNode.removeChild(table);
   }
   FGV.graphs = [];
   renderScene();
}
