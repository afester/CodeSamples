/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

'use strict';

// the global function graph view object
var FGV = {};
var legendTable;

document.getElementByXpath = function(path) {
    return document.evaluate(path, document, null,
            XPathResult.FIRST_ORDERED_NODE_TYPE, null).singleNodeValue;
};


/**
 * Convert a formula as entered by the user into an JavaScript expression TODO:
 * Use a real parser.
 * 
 * @param formula
 *            The formula to convert (e.g. "sin(x)"
 * @returns The corresponding JavaScript expression (e.g. "Math.sin(x)"
 */
function convertFormula(formula) {
    var result = formula;

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
 * Draws a tick on the Y axis.
 */
function drawYTick(y) {
    var ypos, value, textWidth;

    ypos = FGV.y0pos - y / FGV.scalePerPixel;

    if (ypos > 20) { // do not draw a tick nearby the end of the axis!
        FGV.context.moveTo(FGV.x0pos - 5, ypos);
        FGV.context.lineTo(FGV.x0pos + 5, ypos);

        value = Math.round(y * 10) / 10;
        textWidth = FGV.context.measureText(value).width;
        FGV.context.fillText(value, FGV.x0pos - textWidth - 7, ypos + 3);
    }
}

/**
 * Draws a tick on the X axis.
 */
function drawXTick(x) {
    var xpos, value, textWidth;

    xpos = FGV.x0pos + x / FGV.scalePerPixel;
    FGV.context.moveTo(xpos, FGV.y0pos - 5);
    FGV.context.lineTo(xpos, FGV.y0pos + 5);

    value = Math.round(x * 10) / 10;
    textWidth = FGV.context.measureText(value).width;
    FGV.context.fillText(value, xpos - textWidth / 2, FGV.y0pos + 15);
}

/**
 * Draws the axes including the ticks.
 */
function drawAxis() {
    var x, y;

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
    for (y = -FGV.tick; y > FGV.startY; y -= FGV.tick) { // negative axis
        drawYTick(y);
    }
    for (y = FGV.tick; y < FGV.stopY; y += FGV.tick) { // positive axis
        drawYTick(y);
    }

    // draw the x scale
    for (x = -FGV.tick; x > FGV.startX; x -= FGV.tick) { // negative axis
        drawXTick(x);
    }
    for (x = FGV.tick; x < FGV.stopX; x += FGV.tick) { // positive axis
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

/**
 * Draws a specific graph.
 * 
 * @param theGraph
 *            The graph object to draw.
 */
function drawGraph(theGraph) {
    var x, t, y, xpos, ypos, ignore;

    // do not draw the graph if it is in an error state
    if (theGraph.isError) {
        return;
    }

    FGV.context.beginPath();
    FGV.context.strokeStyle = theGraph.color;
    FGV.context.lineWidth = 1;
    FGV.context.setLineDash([]);

    // draw the graph
    x = FGV.startX;
    t = FGV.t; // time parameter - for parametric functions
    y = 0;
    try {
        y = eval(theGraph.formula);
    } catch (e) {
        legendTable.setError(theGraph.index, e);
        theGraph.isError = true;
        return;
    }
    if (y === Infinity) {
        y = 1000;
    }

    xpos = FGV.x0pos + x / FGV.scalePerPixel;
    ypos = FGV.y0pos - y / FGV.scalePerPixel;
    FGV.context.moveTo(xpos, ypos);

    x += FGV.scalePerPixel;
    for (ignore = 0; x < FGV.stopX; x += FGV.scalePerPixel) {
        y = eval(theGraph.formula);

        xpos = FGV.x0pos + x / FGV.scalePerPixel;
        ypos = FGV.y0pos - y / FGV.scalePerPixel;
        FGV.context.lineTo(xpos, ypos);
    }

    FGV.context.stroke();
}

/**
 * Redraws the complete canvas.
 */
function renderScene() {
    var i;

    // clear the canvas
    FGV.context.clearRect(0, 0, FGV.canvas.width, FGV.canvas.height);

    // draw the axes
    drawAxis();

    // draw the graphs
    for (i = 0; i < FGV.graphs.length; i += 1) {
        drawGraph(FGV.graphs[i]);
    }

    // draw the crosshair
    if (FGV.mouseXpos !== -1 && FGV.mouseYpos !== -1) {
        // set visual properties for the cross
        FGV.context.beginPath();
        FGV.context.strokeStyle = "red";
        FGV.context.lineWidth = 0.5;
        FGV.context.setLineDash([ 8, 2 ]);

        // draw the cross
        FGV.context.moveTo(FGV.mouseXpos, 0);
        FGV.context.lineTo(FGV.mouseXpos, FGV.canvas.height);
        FGV.context.moveTo(0, FGV.mouseYpos);
        FGV.context.lineTo(FGV.canvas.width, FGV.mouseYpos);
        FGV.context.stroke();

        // update position values
        FGV.curXposNode.value = Math
                .round(((FGV.mouseXpos - FGV.x0pos) * FGV.scalePerPixel) * 100) / 100;
        FGV.curYposNode.value = Math
                .round(((FGV.y0pos - FGV.mouseYpos) * FGV.scalePerPixel) * 100) / 100;
    } else {
        FGV.curXposNode.value = 'n/a';
        FGV.curYposNode.value = 'n/a';
    }
}


/**
 * This is the table which shows the formulas for the graphs. Each table row can
 * be deleted and edited.
 */
legendTable = {

    /**
     * @param event
     * @return The &lt;tr&gt; element which contains the source element of the
     *         event
     */
    getTrNode : function(event) {
        // retrieve the row number to delete
        if (!event) {
            event = window.event; // Older versions of IE use a global
                                    // reference and not an argument.
        }
        var trNode = (event.target || event.srcElement); // DOM uses
                                                            // 'target'; older
                                                            // versions of IE
                                                            // use 'srcElement'
        while (trNode && trNode.tagName !== 'TR') {
            trNode = trNode.parentNode;
        }

        return trNode;
    },

    /**
     * Renders a table row non-editable
     * 
     * @param rowNumber
     *            The row number to set in no-edit mode
     */
    setRowNoEdit : function(rowNumber) {
        var rowNode, firstButtonCell, secondButtonCell, formulaCell, colorCell, graph;

        // retrieve the tr element and the necessary cells
        rowNode = document.getElementByXpath('//*[@id="_legendTable"]/tr['
                + (rowNumber + 1) + ']');
        firstButtonCell = rowNode.childNodes[0];
        secondButtonCell = rowNode.childNodes[1];
        formulaCell = rowNode.childNodes[2];
        colorCell = rowNode.childNodes[3];

        // set an edit button and a delete button
        firstButtonCell.innerHTML = '<button onclick="legendTable.editGraph()"><img src="pencil.png" /></button>';
        secondButtonCell.innerHTML = '<button onclick="legendTable.removeGraph()"><img src="delete.png" /></button>';

        graph = FGV.graphs[rowNumber];
        formulaCell.innerHTML = '<font color="' + graph.color + '">f<sub>'
                + graph.index + '</sub>(x) = </font>' + graph.text
                + '<font color="red"></font>';

        colorCell.innerHTML = "";
    },

    /**
     * Renders a table row editable.
     * 
     * @param rowNumber
     *            The row number to set in no-edit mode
     */
    setRowEdit : function(rowNumber) {
        var rowNode, firstButtonCell, secondButtonCell, formulaCell, colorCell,
            graph, colors, htmlCode, idx, color, colorName;

        // retrieve the tr element and the necessary cells
        rowNode = document.getElementByXpath('//*[@id="_legendTable"]/tr['
                + (rowNumber + 1) + ']');
        firstButtonCell = rowNode.childNodes[0];
        secondButtonCell = rowNode.childNodes[1];
        formulaCell = rowNode.childNodes[2];
        colorCell = rowNode.childNodes[3];

        graph = FGV.graphs[rowNumber];

        firstButtonCell.innerHTML = '<button onclick="legendTable.cancelEdit()"><img src="cancel.png" /></button>';
        secondButtonCell.innerHTML = '<button onclick="legendTable.commitEdit()"><img src="check_mark.png" /></button>';
        formulaCell.innerHTML = '<font color="'
                + graph.color
                + '">f<sub>'
                + graph.index
                + '</sub>(x) = </font>'
                + '<input onkeypress="legendTable.keyPressed()" size="40" value="'
                + graph.text + '" autofocus="autofocus" />';

        // create the color selector drop down
        colors = [ 'aqua', 'black', 'blue', 'fuchsia', 'gray', 'green',
                'lime', 'maroon', 'navy', 'olive', 'purple', 'red', 'silver',
                'teal', 'white', 'yellow' ];

        htmlCode = '<select>';
        for (idx = 0; idx < colors.length; idx += 1) {
            color = colors[idx];
            colorName = color.charAt(0).toUpperCase() + color.slice(1);
            if (color === graph.color) {
                htmlCode = htmlCode + '<option selected value="' + color + '">'
                        + colorName + '</option>';
            } else {
                htmlCode = htmlCode + '<option value="' + color + '">'
                        + colorName + '</option>';
            }
        }
        htmlCode = htmlCode + '</select>';
        colorCell.innerHTML = htmlCode;
    },

    /**
     * Displays an error for a specific graph.
     * 
     * @param theGraph
     *            The graph for which to display the error.
     * @param theException
     *            The exception to show.
     */
    setError : function(rowNumber, theException) {
        var errorText = document
                .getElementByXpath('//*[@id="_legendTable"]/tr['
                        + (rowNumber + 1) + ']/td[3]/font[2]');
        errorText.innerHTML = '\u00a0\u00a0\u00a0Error: ' + theException;
    },

    /**
     * This function is called when the "commit" button is pressed in a table
     * row.
     * 
     * @param event
     */
    commitEdit : function(event) {
        var trNode, row, graph, inputField, formula, colorField;

        trNode = this.getTrNode(event);
        row = parseFloat(trNode.getAttribute('data-row'));
        graph = FGV.graphs[row];

        // get values from the UI
        inputField = document
                .getElementByXpath('//*[@id="_legendTable"]/tr[' + (row + 1)
                        + ']/td[3]/input');
        formula = inputField.value;
        colorField = document
                .getElementByXpath('//*[@id="_legendTable"]/tr[' + (row + 1)
                        + ']/td[4]/select');

        // set new values
        graph.text = formula;
        graph.formula = convertFormula(formula);
        graph.color = colorField.value;
        graph.isError = false;

        this.setRowNoEdit(row);
        renderScene();
    },

    /**
     * This function is called when the "cancel" button is pressed in a table
     * row.
     * 
     * @param event
     */
    cancelEdit : function(event) {
        var trNode, row;

        trNode = this.getTrNode(event);
        row = parseFloat(trNode.getAttribute('data-row'));

        this.setRowNoEdit(row);
        renderScene();
    },

    /**
     * This function is called when a key is pressed while editing a function.
     * 
     * @param event
     */
    keyPressed : function(event) {
        // retrieve the key code if the pressed key
        if (!event) {
            event = window.event; // Older versions of IE use a global
                                    // reference and not an argument.
        }
        var keyCode = (event.keyCode || event.which); // DOM uses 'target';
                                                        // older versions of IE
                                                        // use 'srcElement'

        if (keyCode === 13) {
            this.commitEdit(event);
        }
    },

    /**
     * This function is called when the "edit" button is pressed in a table row.
     * It sets the corresponding table row into edit mode.
     * 
     * @param event
     *            The event which caused to trigger this function.
     */
    editGraph : function(event) {
        var trNode, row;

        trNode = this.getTrNode(event);
        row = parseFloat(trNode.getAttribute('data-row'));

        // set row into edit mode
        this.setRowEdit(row);
    },

    /**
     * This function is called when the "edit" button is pressed in a table row.
     * It deletes the corresponding table row.
     * 
     * @param event
     *            The event which caused to trigger this function.
     */
    removeGraph : function(event) {
        var trNode, row, i;

        trNode = this.getTrNode(event);
        row = parseFloat(trNode.getAttribute('data-row'));

        // remove graph from graphs array
        for (i = row; i < FGV.graphs.length - 1; i += 1) {
            FGV.graphs[i] = FGV.graphs[i + 1];
            FGV.graphs[i].index = i;
            FGV.graphs[i].isError = false;
        }
        FGV.graphs.pop();

        // recreate the legend table
        this.removeLegend();
        for (i = 0;  i < FGV.graphs.length;  i += 1) {
            FGV.graphs[i].isError = false;
            this.addLegendEntry(FGV.graphs[i]);
        }

        // redraw the graphs
        renderScene();
    },

    /**
     * Adds an entry to the graph legend.
     * 
     * @param graph
     *            The graph object which contains the color, the text and the
     *            JavaScript function of the graph.
     */
    addLegendEntry : function(graph) {
        var table, tr, editTd, deleteTd, td;

        // Get the legend table - the table is always there!
        table = document.getElementById("_legendTable");

        // Note: This approach adds a "tbody" element to each row!!!
        /*
         * var rowHtml = '<tr data-row="' + graph.index + '"><td></td><td></td><td></td><td></td></tr>';
         * table.innerHTML = table.innerHTML + rowHtml;
         */

        // Append a new table row as the legend entry
        tr = document.createElement('tr');
        tr.setAttribute('data-row', graph.index);

        // Create the cells for buttons and for the formula
        editTd = document.createElement('td');
        tr.appendChild(editTd); // first button

        deleteTd = document.createElement('td');
        tr.appendChild(deleteTd); // second button

        td = document.createElement('td');
        tr.appendChild(td); // formula

        td = document.createElement('td');
        tr.appendChild(td); // color

        table.appendChild(tr);

        this.setRowNoEdit(graph.index);
    },

    removeLegend : function() {
        var table = document.getElementById("_legendTable");

        // remove all child nodes from the table
        table.innerHTML = '';
    }
};


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
        FGV.startY = -(xRange * FGV.canvas.height)
                / (2 * (FGV.canvas.width - FGV.paddingLeft));
        FGV.stopY = -FGV.startY;

        // calculate the scale factor
        FGV.scalePerPixel = (FGV.stopX - FGV.startX)
                / (FGV.canvas.width - FGV.paddingLeft);

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
 * Adds a graph to the graph array and to the legend, but does not draw the
 * graph yet.
 * 
 * @param formula
 *            The function to add.
 * @param color
 *            The color for the function graph.
 */
function addGraph(formula, color) {

    var graph = {
        'index' : FGV.graphs.length,
        'text' : formula,
        'formula' : convertFormula(formula),
        'color' : color,
        'isError' : false,

        toString : function() {
            return 'Graph[' + this.index + ', "' + this.text + '", '
                    + this.color + ', ' + this.isError + ']';
        }
    };

    FGV.graphs.push(graph);
    legendTable.addLegendEntry(graph);
}


/**
 * Event function for the canvas mouse move events. Updates the crosshair
 * coordinates when the mouse moves.
 * 
 * @param evt
 *            The event object
 */
function onMouseMove(evt) {
    FGV.mouseXpos = evt.clientX - FGV.canvas.offsetLeft;
    FGV.mouseYpos = evt.clientY - FGV.canvas.offsetTop;

    renderScene();
}

/**
 * Event function for the canvas mouse out events. Whenever the mouse leaves the
 * canvas, the crosshair is removed and the crosshair coordinates are set to
 * "n/a".
 * 
 * @param evt
 *            The event object
 */
function onMouseOut() {
    FGV.mouseXpos = -1;
    FGV.mouseYpos = -1;

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
    FGV.fromX = document.getElementById("_fromX");
    FGV.toX = document.getElementById("_toX");
    FGV.fromY = document.getElementById("_fromY");
    FGV.toY = document.getElementById("_toY");
    FGV.canvas = document.getElementById("_canvas");
    FGV.curXposNode = document.getElementById("_curXpos");
    FGV.curYposNode = document.getElementById("_curYpos");
    FGV.legendDiv = document.getElementById("_legend");
    FGV.curTNode = document.getElementById("_curT");
    FGV.curDTNode = document.getElementById("_curDT");
    FGV.fromTNode = document.getElementById("_fromT");
    FGV.toTNode = document.getElementById("_toT");

    FGV.canvas.onmousemove = onMouseMove;
    FGV.canvas.onmouseout = onMouseOut;
    FGV.context = FGV.canvas.getContext('2d');
    FGV.context.font = '8pt arial,sans-serif';

    FGV.animationRate = 40; // animation refresh rate in ms
    FGV.mouseXpos = -1; // X mouse position for crosshair
    FGV.mouseYpos = -1; // Y mouse position for crosshair

    // Not all browsers support setLineDash!
    // http://www.rgraph.net/blog/2013/january/html5-canvas-dashed-lines.html
    if (!FGV.context.setLineDash) {
        FGV.context.setLineDash = function() {
        };
    }

    FGV.graphs = [];
    FGV.dt = 0;
    FGV.t = 0;
    FGV.animate = false;

    // Some sample graphs, especially for debugging purposes
    addGraph("2*sin(x+2*t)", "red");
    addGraph("log(X)+t", "blue");
    addGraph("exp(x+t)", "green");
    addGraph("x + 3*pow(x-t, 2) + pow(x, 3) - 1", "maroon");

    updateValues();
    renderScene();
}

/**
 * Action function for the "Add" button.
 */
function addAction() {
    addGraph('', 'aqua');

    // set row into edit mode
    legendTable.setRowEdit(FGV.graphs.length - 1);
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
    legendTable.removeLegend();
    FGV.graphs = [];
    renderScene();
}

/**
 * Action function for the "Apply" button of the parameter for parametric
 * functions.
 */
function applyParameter() {
    FGV.t = parseFloat(FGV.curTNode.value);
    renderScene();
}

function nextStep() {
    // calculate next value
    FGV.t = FGV.t + FGV.dt;

    // check if value still in range - if end is reached, reset parameter
    if (FGV.dt < 0) {
        if (FGV.t <= FGV.stopT) {
            // FGV.animate = false;
            FGV.t = parseFloat(FGV.fromTNode.value);
        }
    } else {
        if (FGV.t >= FGV.stopT) {
            // FGV.animate = false;
            FGV.t = parseFloat(FGV.fromTNode.value);
        }
    }

    // Update UI
    FGV.curTNode.value = Math.round(FGV.t * 100) / 100;
    renderScene();
}

function previousStep() {
    // calculate previous value
    FGV.t = FGV.t - FGV.dt;

    // check if value still in range - if start is reached, reset parameter
    if (FGV.dt < 0) {
        if (FGV.t >= FGV.startT) {
            FGV.t = parseFloat(FGV.toTNode.value);
        }
    } else {
        if (FGV.t <= FGV.startT) {
            FGV.t = parseFloat(FGV.toTNode.value);
        }
    }

    // Update UI
    FGV.curTNode.value = Math.round(FGV.t * 100) / 100;
    renderScene();
}

/**
 * Action function for the "Start" button to start the animation of parametric
 * functions.
 */
function startAnimation() {
    if (FGV.animate === true) {
        return;
    }

    var step = function() {
        if (FGV.animate === true) {
            nextStep();

            // schedule next step
            setTimeout(step, FGV.animationRate);
        }
    };

    // initialize values from user input
    FGV.dt = parseFloat(FGV.curDTNode.value);
    FGV.startT = parseFloat(FGV.fromTNode.value);
    FGV.stopT = parseFloat(FGV.toTNode.value);

    FGV.t = FGV.startT;
    FGV.animate = true;

    // Update UI
    FGV.curTNode.value = Math.round(FGV.t * 100) / 100;
    renderScene();

    // schedule next step
    setTimeout(step, FGV.animationRate);
}

/**
 * Action function for the "Stop" button to stop the animation of parametric
 * functions.
 */
function stopAnimation() {
    FGV.animate = false;
}

/**
 * Action function for the "Increment" button to advance to the next frame.
 */
function next() {
    // initialize values from user input
    FGV.dt = parseFloat(FGV.curDTNode.value);
    FGV.startT = parseFloat(FGV.fromTNode.value);
    FGV.stopT = parseFloat(FGV.toTNode.value);
    FGV.animate = false;

    nextStep();
}

/**
 * Action function for the "Decrement" button to go back to the previous frame.
 */
function previous() {
    // initialize values from user input
    FGV.dt = parseFloat(FGV.curDTNode.value);
    FGV.startT = parseFloat(FGV.fromTNode.value);
    FGV.stopT = parseFloat(FGV.toTNode.value);
    FGV.animate = false;

    previousStep();
}
