/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

import {variablesAction} from "./variables";
import {debugConsole, clearProxy} from "./tools";
import {jquerySampleAction} from "./samples";

var myPoint, value;

if (typeof Object.create !== 'function') {
    Object.create = function (o) {
        var F = function () {};
        F.prototype = o;
        return new F();
    };
}

var Point = {
        "x" : 0,

        "y" : 0,

        "toString" :
            function() {
                return "Point[" + this.x + ", " + this.y + "]";
            }
    };


function dumpObjectRec(indent, objName, obj) {
    var ownProperties,
        prefix = '                    '.substring(0, indent),
        name,
        idx;

    debugConsole.writeln("\n" + prefix + "Properties of " + typeof obj + " " + objName + ":\n" + prefix + "----------------------------");

    for (name in obj) {
        debugConsole.writeln(prefix + "  " + name + " (" + typeof name + ") own property: " + obj.hasOwnProperty(name) + " = " + obj[name]);
    }

    ownProperties = Object.getOwnPropertyNames(obj);
    if (ownProperties.length > 0) {
        debugConsole.writeln("\n" + prefix + "  Own properties:\n" + prefix + "  -------------------------");

        for (idx in ownProperties) {
            name = ownProperties[idx];
            debugConsole.writeln(prefix + "  " + name + " (" + typeof obj[name] + ") = " + obj[name]);

            if (obj[name] !== null && typeof obj[name] === 'object') {
                dumpObjectRec(indent + 2, name, obj[name]);
            }
        }
    }
}


function dumpObject(objName, obj) {
    dumpObjectRec(0, objName, obj);
}


export function dumpAction() {
    debugConsole.appendText("Hello World!!\n");

    myPoint = Object.create(Point);
    debugConsole.writeln(Point.toString() + myPoint.toString());

    myPoint.x = 4;

    debugConsole.writeln(Point.toString() + myPoint.toString());
    debugConsole.writeln(Point.hasOwnProperty('toString'));
    debugConsole.writeln(myPoint.hasOwnProperty('toString'));

    dumpObject('Point', Point);
    dumpObject('myPoint', myPoint);
    delete myPoint.x;
    dumpObject('myPoint', myPoint);

    dumpObject('dumpAction', dumpAction);
    dumpObject('Function', Function);
    dumpObject('Object', Object);
    dumpObject('Number', Object);
    dumpObject('String', Object);
}


function functionAction() {
    var add, result, someObject, someOtherObject, SomeValue, value1, value2, calculateAvg;

    add = function (a, b) {
        return a + b;
    };

    // add is a reference to a function object, so it can be invoked
    result = add(5, 7);

    debugConsole.writeln("5 + 7 = " + result);

    /** Method Invocation Pattern - "this" refers to the current object **/
    someObject = {
        'value' : 'SomeValue',

        'invokeMethod' : function() {
            debugConsole.writeln(this.value + "(" + typeof this.value + ")");
        }
    };
    someObject.invokeMethod();

    someOtherObject = {
        'value' : 4.5,

        'invokeMethod' : someObject.invokeMethod  // same function can be attached to multiple objects
    };
    someOtherObject.invokeMethod();

    /** Function invocation pattern: "this" refers to the global context **/
    someOtherObject.setDefaultValue = function () {

        var someFunction = function () {
                this.value = 1.5;    // Attention! when invoked as a function, "this" refers to the global context!
            };

        someFunction();
    };
    someOtherObject.setDefaultValue();
    debugConsole.writeln("someOtherObject.value: " + someOtherObject.value); // still 4.5!
    debugConsole.writeln("global value: " + value);                          // new global property: 1.5!!

    someOtherObject.setDefaultValue2 = function () {
        var that, someFunction2;

        that = this;
        someFunction2 = function () {
            that.value = 1.8;
        };

        someFunction2();
    };
    someOtherObject.setDefaultValue2();
    debugConsole.writeln("someOtherObject.value: " + someOtherObject.value); // Now correctly 1.8
    debugConsole.writeln("global value: " + value);                          // global property is still 1.5

    /** Constructor Invocation Pattern */
    SomeValue = function (aValue) {    // constructor function
        this.value = aValue;
    };

    SomeValue.prototype.getValue = function() {
        return this.value;
    };

    value1 = new SomeValue("Hello");
    value2 = new SomeValue("World");
    debugConsole.writeln("value1: " + value1.getValue());
    debugConsole.writeln("value2: " + value2.getValue());

    /** Apply invocation pattern */
    // each function has the "apply" method which can be used to invoke the function.
    // the first parameter is the object which will be passed to the function
    // as "this" parameter, the optional second parameter are the parameters passed to the function.
    result = SomeValue.prototype.getValue.apply(value1);
    debugConsole.writeln("value1 through apply(): " + result);

    /** The arguments parameter - allow access to all parameters */
    calculateAvg = function() {
        var sum = 0, idx;
        for (idx = 0;  idx < arguments.length;  idx += 1) {
            sum += arguments[idx];
        }
        return sum / arguments.length;
    };
    debugConsole.writeln("Average: " + calculateAvg(3, 4));

    /** ********************************************************* */

    // Add a method "method" to all functions
    Function.prototype.method = function (name, func) {
        this.prototype[name] = func;
        return this;
    };

    // Thus, we can now add methods to a function with
    // myFunc.method(someMethod, function() {...});
    // instead of
    // myFunc.prototype.someMethod = function() {...};

    Function.prototype.someFunction = function() {
        return "someFunction";
    };
    debugConsole.writeln(calculateAvg.someFunction());

    Function.method('otherFunction', function() { return "otherFunction"; });
    debugConsole.writeln(calculateAvg.otherFunction());

    /** Add an average() function to Math */

    Math.avg = function() {
        var sum = 0, idx;
        for (idx = 0;  idx < arguments.length;  idx += 1) {
            sum += arguments[idx];
        }
        return sum / arguments.length;
    };

    debugConsole.writeln(Math.avg(1, 3, 4, 5));
}


function exceptionAction() {
    var calculateAvg, count, f1;

    calculateAvg = function() {
        if (arguments.length === 0) {
            throw {
                name : 'DivByZero',
                message: 'Number of parameters is 0'
            };
        }

        var sum = 0, idx;
        for (idx = 0;  idx < arguments.length;  idx += 1) {
            sum += arguments[idx];
        }
        return sum / arguments.length;
    };

    try {
        debugConsole.writeln("Average: " + calculateAvg());
    } catch (e) {
        dumpObject('e', e);
    }

    count = 0;
    try {
        f1 = function f2() {
            count += 1;
            f2();
        };
        f1();
    } catch (e) {
        debugConsole.writeln("Stack depth: " + count);
        dumpObject('e', e);
    }
}


function traverseDOM(level, node, func) {
    func(level, node);
    node = node.firstChild;
    while (node) {
        traverseDOM(level + 1, node, func);
        node = node.nextSibling;
    }
}


function traverseDOMAction() {
    var nodeTypes = ['Element',
                     'Attribute',
                     'Text',
                     'CDATA',
                     'Entity reference',
                     'Entity',
                     'Processing directive',
                     'Comment',
                     'Document',
                     'Document type',
                     'Document fragment',
                     'Notation'];

    traverseDOM(0, document, function(level, node) {
        var prefix = '                    '.substring(0, 2 * level);
        debugConsole.writeln(prefix + nodeTypes[node.nodeType - 1] + " " + node.nodeName);
    });
}


function colorSampleAction() {
    var fade, coloredNode;

    fade = function (node) {
        var level = 1, step;

        step = function () {
            var hex = level.toString(16);
            node.style.backgroundColor = '#' + hex + hex + hex; // FFFF' + hex + hex;
            if (level < 15) {
                level += 1;
                setTimeout(step, 100);
            }
        };
        setTimeout(step, 100);
    };

    coloredNode = document.getElementById('_textArea');
    fade(coloredNode);
}


function arraySampleAction() {
    var a, a1, a2, i, someArray, someArray2, someArray3, someArray4, array1, array2, array3, result, array4;

    a = [1, 2, 3];
    dumpObject("a", a);

    a[1] = a[2];
    dumpObject("a", a);


    a1 = [ {name: "first"}, {name: "second"}, {name: "third"}];
    dumpObject("a1", a1);
    a1[1] = a1[2];
    dumpObject("a1", a1);


    a2 = [ {name: "first"}, {name: "second"}, {name: "third"}, {name: "fourth"}];
    for (i = 1;  i < a2.length - 1;  i += 1) {
        a2[i] = a2[i + 1];
    }
    dumpObject("a2", a2);


    someArray = ['a', 'b', 'c'];
    debugConsole.writeln('Array length:' + someArray.length);
    someArray[5] = 'e';
    debugConsole.writeln('Array length:' + someArray.length + ',' + someArray[3]);
    debugConsole.writeln('=========================');

    someArray2 = ['a', 'b', 'c', 'd', 'e'];
    debugConsole.writeln('someArray2: ' + someArray2);

    someArray3 = someArray2.slice(1, 3);
    debugConsole.writeln('someArray2.slice(1, 3): ' + someArray3);

    debugConsole.writeln('someArray2: ' + someArray2);

    someArray4 = someArray2.splice(2, 1);
    debugConsole.writeln('someArray2.splice(2, 1): ' + someArray4);
    debugConsole.writeln('someArray2: ' + someArray2 + '(' + someArray2.length + ')');

    debugConsole.writeln('\narray.concat()\n=======================================================');
    array1 = [16, 4, 13, 5];
    array2 = [2, 21, 7];
    array3 = array1.concat(array2, 'Hello', 'World');
    debugConsole.writeln('   array1: ' + array1);
    debugConsole.writeln('   array2: ' + array2);
    debugConsole.writeln('   array3: ' + array3);

    debugConsole.writeln('\narray.join()\n=======================================================');
    result = array3.join('|');
    debugConsole.writeln('   array3: ' + result);

    debugConsole.writeln('\narray.push()\n=======================================================');
    array3.push('Moon', 'Sun');
    debugConsole.writeln('   array3: ' + array3);

    debugConsole.writeln('\narray.pop()\n=======================================================');
    array3.pop();
    debugConsole.writeln('   array3: ' + array3);

    debugConsole.writeln('\narray.reverse()\n=======================================================');
    array3.reverse();
    debugConsole.writeln('   array3: ' + array3);

    debugConsole.writeln('\narray.shift()\n=======================================================');
    array3.shift();
    debugConsole.writeln('   array3: ' + array3);

    debugConsole.writeln('\narray.unshift()\n=======================================================');
    array3.unshift(42);
    debugConsole.writeln('   array3: ' + array3);

    debugConsole.writeln('\narray.slice()\n=======================================================');
    array4 = array3.slice(3, 8);
    debugConsole.writeln('   array3: ' + array3);
    debugConsole.writeln('   array4: ' + array4);

    debugConsole.writeln('\narray.sort()\n=======================================================');
    array4.sort();
    debugConsole.writeln('   array4: ' + array4);
    array4.sort(function (a, b) {
        return a - b;
    });
    debugConsole.writeln('   array4: ' + array4);

    debugConsole.writeln('\narray.splice()\n=======================================================');
    array4.splice(2, 2, 'Moon');
    debugConsole.writeln('   array4: ' + array4);
}


function regexpSampleAction() {
    var parseUrl, someUrl, result, proto, host, port, path, params, fragment;

    parseUrl = /^(?:([A-Za-z]+):)?(\/{0,3})([0-9.\-A-Za-z]+)(?::(\d+))?(?:\/([^?#]*))?(?:\?([^#]*))?(?:#(.*))?$/;
    debugConsole.writeln(parseUrl);

    someUrl = "http://www.example.com:8080/path/path2?param=1&param2=2#fragment";
    result = parseUrl.exec(someUrl);

    proto = result[1];
    host = result[3];
    port = result[4];
    path = result[5];
    params = result[6];
    fragment = result[7];

    debugConsole.writeln('Protocol  : ' + proto);
    debugConsole.writeln('Host      : ' + host);
    debugConsole.writeln('Port      : ' + port);
    debugConsole.writeln('Path      : ' + path);
    debugConsole.writeln('Parameters: ' + params);
    debugConsole.writeln('Fragment  : ' + fragment);

    dumpObject('result', result);
}


/* Class is syntactic sugar over the prototype mechanism */
class SampleClass {
	constructor(x, y) {
		this.x = x;
		this.y = y;
	}

	toString() {
		return "x=" + this.x + ", y=" + this.y;
	}
}


function classSampleAction() {
    debugConsole.writeln("class Sample");

	var p = new SampleClass(45, 98);
	debugConsole.writeln("Point:" + p.toString());
}

document.querySelector('#vads').addEventListener('click', variablesAction);
document.querySelector('#_clearButton').addEventListener('click', clearProxy);
document.querySelector('#doa').addEventListener('click', dumpAction);
document.querySelector('#foa').addEventListener('click', functionAction);
document.querySelector('#exa').addEventListener('click', exceptionAction);
document.querySelector('#tda').addEventListener('click', traverseDOMAction);
document.querySelector('#csa').addEventListener('click', colorSampleAction);
document.querySelector('#asa').addEventListener('click', arraySampleAction);
document.querySelector('#rsa').addEventListener('click', regexpSampleAction);
document.querySelector('#clsa').addEventListener('click', classSampleAction);
document.querySelector('#jqs').addEventListener('click', jquerySampleAction);
