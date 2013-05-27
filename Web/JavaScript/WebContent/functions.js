


// the global function graph view object
var FGV = {};

function convertFormula(formula) {

	result = formula.replace(/sin/g, "Math.sin");
	result = result.replace(/cos/g, "Math.cos");
	result = result.replace(/tan/g, "Math.tan");
    result = result.replace(/pow/g, "Math.pow");
    result = result.replace(/log/g, "Math.log");
    result = result.replace(/exp/g, "Math.exp");

	return result;
}


function updateValues() {
	var xRange = FGV.toX.value - FGV.fromX.value;
	FGV.startY = -(xRange * 460) / (2 * 620);
	FGV.stopY = -FGV.startY;

	FGV.fromY.value = Math.round(FGV.startY * 100) / 100;
	FGV.toY.value = Math.round(FGV.stopY * 100) / 100;

	FGV.startX = parseFloat(FGV.fromX.value);
    FGV.stopX = parseFloat(FGV.toX.value);

    FGV.scalePerPixel = (FGV.stopX - FGV.startX) / 620;

    FGV.x0pos = -FGV.startX / FGV.scalePerPixel + 10;
    FGV.y0pos = -FGV.startY / FGV.scalePerPixel + 10;

    // calculate the distance for each tick
    tick = 20 * FGV.scalePerPixel;
    if (tick < 0.1) {
    	tick = 0.1;
    } else if (tick < 0.5) {
    	tick = 0.5;
    } else if (tick < 1.0) {
    	tick = 1.0;
    }
}


/**
 * Draws the axis including the ticks. 
 */
function drawAxis() {
	// set the axis visual properties
	FGV.context.beginPath();
	FGV.context.strokeStyle = "black";
	FGV.context.lineWidth = 1;
	FGV.context.setLineDash([]);

	// draw the axis lines

	// Y axis
	FGV.context.moveTo(FGV.x0pos, 470);
	FGV.context.lineTo(FGV.x0pos, 0);
    FGV.context.lineTo(FGV.x0pos - 5, 10);
    FGV.context.lineTo(FGV.x0pos + 5, 10);
    FGV.context.lineTo(FGV.x0pos, 0);
    FGV.context.strokeText("Y", FGV.x0pos - 15, 12);

    // X axis
	FGV.context.moveTo(10, FGV.y0pos);
	FGV.context.lineTo(640, FGV.y0pos);
	FGV.context.lineTo(630, FGV.y0pos - 5);
	FGV.context.lineTo(630, FGV.y0pos + 5);
    FGV.context.lineTo(640, FGV.y0pos);
    FGV.context.strokeText("X", 630, FGV.y0pos + 20);

    // draw the y scale
    for (var y = -tick; y > FGV.startY; y -= tick) {
    	var ypos = FGV.y0pos - y / FGV.scalePerPixel;
    	FGV.context.moveTo(FGV.x0pos - 5, ypos); 
    	FGV.context.lineTo(FGV.x0pos + 5, ypos);

    	var value = Math.round(y * 10) / 10;
    	var textWidth = FGV.context.measureText(value).width;
        FGV.context.fillText(value, FGV.x0pos - textWidth - 7, ypos + 3);
    }
    for (var y = tick; y < FGV.stopY; y += tick) {
        var ypos = FGV.y0pos - y / FGV.scalePerPixel;
        FGV.context.moveTo(FGV.x0pos - 5, ypos); 
        FGV.context.lineTo(FGV.x0pos + 5, ypos);

        var value = Math.round(y * 10) / 10;
    	var textWidth = FGV.context.measureText(value).width;
        FGV.context.fillText(value, FGV.x0pos - textWidth - 7, ypos + 3);
    }

    // draw the x scale
    for (var x = -tick; x > FGV.startX; x -= tick) {
        var xpos = FGV.x0pos + x / FGV.scalePerPixel;
        FGV.context.moveTo(xpos, FGV.y0pos - 5); 
        FGV.context.lineTo(xpos, FGV.y0pos + 5);

        var value = Math.round(x * 10) / 10;
    	var textWidth = FGV.context.measureText(value).width;
        FGV.context.fillText(value, xpos - textWidth / 2, FGV.y0pos + 15);
    }
    for (var x = tick; x < FGV.stopX; x += tick) {
    	var xpos = FGV.x0pos + x / FGV.scalePerPixel;
    	FGV.context.moveTo(xpos, FGV.y0pos - 5); 
    	FGV.context.lineTo(xpos, FGV.y0pos + 5);

        var value = Math.round(x * 10) / 10;
    	var textWidth = FGV.context.measureText(value).width;
        FGV.context.fillText(value, xpos - textWidth / 2, FGV.y0pos + 15);
    }

    FGV.context.fill(); // ????? Probably not quite  correct, but it works ...
    FGV.context.stroke();   // still required!!!!
}

/**
 * Draw all graphs which are currently defined.
 */
function drawGraph(theGraph) {
	FGV.context.beginPath();
	FGV.context.strokeStyle = theGraph.color;
	FGV.context.lineWidth = 1;
	FGV.context.setLineDash([]);

    // draw the graph
    var x = FGV.startX;
    var y = eval(theGraph.formula);
    if (y == Infinity) {
        y = 1000;
    }

    var xpos = FGV.x0pos + x / FGV.scalePerPixel;
    var ypos = FGV.y0pos - y / FGV.scalePerPixel;
    FGV.context.moveTo(xpos, ypos);

    x += FGV.scalePerPixel;
    for (  ;  x < FGV.stopX;  x += FGV.scalePerPixel) {
        var y = eval(theGraph.formula);

        var xpos = FGV.x0pos + x / FGV.scalePerPixel;
        var ypos = FGV.y0pos - y / FGV.scalePerPixel;
        FGV.context.lineTo(xpos, ypos);
    }
    FGV.context.stroke();
}


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

	// create a text node containing the function name, colored in the graph's color
	var font = document.createElement('font');
	font.setAttribute('color', graph.color);
	var fnNameNode = document.createTextNode('f' + FGV.graphs.length + '(x) = ');
    font.appendChild(fnNameNode);

	// create a text node containing the actual function
    var fnTextNode = document.createTextNode(graph.text);

    td.appendChild(font);
    td.appendChild(fnTextNode);

    table.appendChild(tr);
}


/**
 * Adds a graph to the graph array and to the legend, but does not 
 * draw the graph yet.
 *
 * @param formula	The function to add.
 * @param color		The color for the function graph.
 */
function addGraphFor(formula, color) {

	var graph = {
            "text"    : formula,
			"formula" : convertFormula(formula),
			"color"   : color 
	};

	FGV.graphs.push(graph);
	addLegendEntry(graph);
}


function renderScene() {
    FGV.context.clearRect(0, 0, FGV.canvas.width, FGV.canvas.height);

    drawAxis();

    for (var i = 0; i < FGV.graphs.length; i++) {
        drawGraph(FGV.graphs[i]);
    }
}


function onMouseMove(evt) {
	  var mouseXpos = evt.clientX - FGV.canvas.offsetLeft;
	  var mouseYpos = evt.clientY - FGV.canvas.offsetTop;

	  renderScene();

	  // set visual properties for the cross
	  FGV.context.beginPath();
	  FGV.context.strokeStyle = "red";
	  FGV.context.lineWidth = 0.5;
	  FGV.context.setLineDash([8,2]);

	  // draw the cross
	  FGV.context.moveTo(mouseXpos, 0);
	  FGV.context.lineTo(mouseXpos, 480);
	  FGV.context.moveTo(0, mouseYpos);
	  FGV.context.lineTo(640, mouseYpos);
	  FGV.context.stroke();

	  // update position values
	  FGV.curXposNode.value = Math.round(((mouseXpos - FGV.x0pos) * FGV.scalePerPixel)*100)/100;
      FGV.curYposNode.value = Math.round(((FGV.y0pos - mouseYpos) * FGV.scalePerPixel)*100)/100;
}


function onMouseOut(evt) {
    FGV.curXposNode.value = 'n/a';
    FGV.curYposNode.value = 'n/a';
	renderScene();
}


/**
 * Event function for the "onload" event. This function is called when the
 * page has been loaded.
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
    FGV.legendDiv= document.getElementById("_legend");

    FGV.canvas.onmousemove = onMouseMove;
    FGV.canvas.onmouseout = onMouseOut;
    FGV.context = FGV.canvas.getContext('2d');
    FGV.context.font = '8pt arial,sans-serif';

    // Not all browsers support setLineDash!
    // http://www.rgraph.net/blog/2013/january/html5-canvas-dashed-lines.html
    if (!FGV.context.setLineDash) {
        FGV.context.setLineDash = function () {};
    }

    FGV.graphs = [];

    // Some sample graphs, especially for debugging purposes
    addGraphFor("2*sin(x)", "red");
    addGraphFor("log(x)", "blue");
    addGraphFor("exp(x)", "green");
    addGraphFor("x + 3*pow(x, 2) + pow(x, 3) - 1", "maroon");

	reset();
}


/**
 * Action function for the "Add" button.
 */
function addGraph() {
	addGraphFor(FGV.formula.value, FGV.graphColor.value);
    renderScene(); // drawGraph(graph);
}


/**
 * Action function for the "Apply" button on the X axis.
 */
function reset() {
    updateValues();
    renderScene();
}


/**
 * Action function for the "Clear all" button.
 */
function clearDiagram() {
	// remove the legend table
    var table = document.getElementById("_legendTable");
    if (table) {
    	table.parentNode.removeChild(table);
    }
    FGV.graphs = [];
    renderScene();
}
