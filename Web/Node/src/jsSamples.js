/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

import {clearProxy, debugConsole, dumpObject} from "./tools";
import {enhancedVariablesAction, variablesAction} from "./variables";
import {
    asyncExample,
    functionalExamples,
    initializeClock,
    jquerySampleAction,
    promiseExample,
    yieldExample
} from "./samples";

import importExample, {someExport} from "./exportSample";

let myPoint, value;

if (typeof Object.create !== 'function') {
    Object.create = function (o) {
        const F = function () {};
        F.prototype = o;
        return new F();
    };
}

const Point = {
        "x" : 0,

        "y" : 0,

    "toString" :
            function() {
                return "Point[" + this.x + ", " + this.y + "]";
            }
    };

function dumpAction() {
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

function globFunThis() {
    // return this;
    debugConsole.dumpObjectFlat(this);
}

function objectAction() {
    debugConsole.writeln("JavaScript objects\n====================");

    const obj1 = {}
    dumpObject('obj1', obj1);

    const obj2 = { name: "John", age: 42}
    dumpObject('obj2', obj2);

    const myFunc = function(x) {return 2*x;}

    const obj3_1 = {id: 1234, value: 'Hello'};
    const obj3_2 = {id: 2345, value: 'World'};
    const obj3_3 = [1, 3, 5, 7, 11, 13, 17, 23];
    const obj3 = {
        obj3_1,
        obj3_2,
        obj3_3,
        myFunc
    };
    dumpObject('obj3', obj3);

    const obj4 = {
            first: {
                name: "sun",
                diameter: 1234567
            },
            second: {
                name: "moon",
                diameter: 3000
            }
    };
    dumpObject('obj4', obj4);

    const obj5 = {
            1: {
                "name": "sun",
                diameter: 1234567
            },
            2: {
                name: "moon",
                "diameter": 3000
            }
    };
    dumpObject('obj5', obj5);

    debugConsole.writeln("JavaScript this keyword\n====================");
    const test = {
      prop: 42,
      func: function() {
        return this.prop;
      },
    };

    debugConsole.writeln("RESULT: " + test.func())

    const isOrNot = globalThis === window;
    debugConsole.writeln("IsGlobalWindow: " + isOrNot);

    // in a pure global function, this is undefined (which somewhat makes sense) - some older browser
    // implementations incorrectly set this to window in strict mode!
    debugConsole.writeln("this of a global function:");    // undefined
    globFunThis();

    const ctx = {value: 35};

    debugConsole.writeln("this of a function called with call:");
    globFunThis.call(ctx);
    debugConsole.writeln("this of a function called with apply:");
    globFunThis.apply(ctx);

    debugConsole.writeln("this of a global function bound to an object:");
    globFunThis = globFunThis.bind(ctx);
    globFunThis();
}


function functionAction() {

    /** Function expression */
    const add = function (a, b) {
        return a + b;
    };

    // add is a reference to a function object, so it can be invoked
    const result = add(5, 7);

    debugConsole.writeln("5 + 7 = " + result);

    /** Method Invocation Pattern - "this" refers to the current object **/
    const someObject = {
        'value' : 'SomeValue',

        'invokeMethod' : function() {
            debugConsole.writeln(this.value + "(" + typeof this.value + ")");
        }
    };
    someObject.invokeMethod();

    const someOtherObject = {
        'value' : 4.5,

        'invokeMethod' : someObject.invokeMethod  // same function can be attached to multiple objects
    };
    someOtherObject.invokeMethod();

    /** Function invocation pattern: "this" refers to the global context **/
    // Note: this should be undefined here, because someFunction was called directly and not as a method or
    // property of an object. This feature wasn't implemented in some browsers when they first started to
    // support strict mode. As a result, they incorrectly returned the window object.

    // someOtherObject.setDefaultValue = function () {
    //
    //     var someFunction = function () {
    //             this.value = 1.5;    // (Attention! when invoked as a function, "this" refers to the global context!)
    //         };
    //
    //     someFunction();
    // };
    // someOtherObject.setDefaultValue();
    // debugConsole.writeln("someOtherObject.value: " + someOtherObject.value); // still 4.5!
    // debugConsole.writeln("global value: " + value);                          // new global property: 1.5!!

    someOtherObject.setDefaultValue2 = function () {
        let that, someFunction2;

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
    const SomeValue = function (aValue) {    // constructor function
        this.value = aValue;
    };

    SomeValue.prototype.getValue = function() {
        return this.value;
    };

    const value1 = new SomeValue("Hello");
    const value2 = new SomeValue("World");
    debugConsole.writeln("value1: " + value1.getValue());
    debugConsole.writeln("value2: " + value2.getValue());

    /** Apply invocation pattern */
    // each function has the "apply" method which can be used to invoke the function.
    // the first parameter is the object which will be passed to the function
    // as "this" parameter, the optional second parameter are the parameters passed to the function.
    const result2 = SomeValue.prototype.getValue.apply(value1);
    debugConsole.writeln("value1 through apply(): " + result2);

    /** The arguments parameter - allow access to all parameters */
    const calculateAvg = function() {
        debugConsole.writeln("Is arguments an array?" + Array.isArray(arguments));
        let sum = 0, idx;
        for (idx = 0;  idx < arguments.length;  idx += 1) {
            sum += arguments[idx];
        }
        return sum / arguments.length;
    };
    debugConsole.writeln("Average: " + calculateAvg(3, 4, 5, 6));

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
        let sum = 0, idx;
        for (idx = 0;  idx < arguments.length;  idx += 1) {
            sum += arguments[idx];
        }
        return sum / arguments.length;
    };

    debugConsole.writeln(Math.avg(1, 3, 4, 5));

    debugConsole.writeln("\nDefault parameters\n========================");
    function sayHello(name="World") {
        debugConsole.writeln(`Hello ${name}`)
    }
    sayHello();
    sayHello("Moon");

    debugConsole.writeln("\nArrow function\n========================");
    const arrowFunc = name => `Hello ${name}`;
    debugConsole.writeln(arrowFunc('Mars'));

    // Advanced syntax: use parantheses when returning object literals
    const arrowFunc2 = (firstname, lastname) => ({
            first: firstname,
            last: lastname
        });
    debugConsole.writeln(arrowFunc2('Peter', 'Meier').first);

    const values = [11, 13, 17];
    function test (alfa, bravo, charlie) {
        // debugConsole.writeln(alfa);
        // debugConsole.writeln(bravo);
        // debugConsole.writeln(charlie);

        // [alfa, bravo, charlie].forEach(function (value) {
        //     debugConsole.writeln(value);
        // });

        [alfa, bravo, charlie].forEach(value => debugConsole.writeln(value));
    }
    // test.apply(null, values);
    test(...values);

    const setValues = new Set([11, 13, 17]);
    // test.apply(null, setValues);   // does not even work!
    test(...setValues);
}


function exceptionAction() {
    let count;

    const calculateAvg = function() {
        if (arguments.length === 0) {
            throw {
                name : 'DivByZero',
                message: 'Number of parameters is 0'
            };
        }

        let sum = 0, idx;
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
        const f1 = function f2() {
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
    const nodeTypes = ['Element',
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
        const prefix = '                    '.substring(0, 2 * level);
        debugConsole.writeln(prefix + nodeTypes[node.nodeType - 1] + " " + node.nodeName);
    });
}


function colorSampleAction() {
    const fade = function (node) {
        let level = 1, step;

        step = function () {
            const hex = level.toString(16);
            node.style.backgroundColor = '#' + hex + hex + hex; // FFFF' + hex + hex;
            if (level < 15) {
                level += 1;
                setTimeout(step, 100);
            }
        };
        setTimeout(step, 100);
    };

    const coloredNode = document.getElementById('_textArea');
    fade(coloredNode);
}


function arraySampleAction() {
    let a, a1, a2, i, someArray, someArray2, someArray3, someArray4, array1, array2, array3, result, array4;

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

    debugConsole.writeln('\narray.destructuring()\n=======================================================');
    const [,,destruct] = array4;
    debugConsole.writeln('   destruct: ' + destruct);
}


function regexpSampleAction() {
    const parseUrl = /^(?:([A-Za-z]+):)?(\/{0,3})([0-9.\-A-Za-z]+)(?::(\d+))?(?:\/([^?#]*))?(?:\?([^#]*))?(?:#(.*))?$/;
    debugConsole.writeln(parseUrl);

    const someUrl = "https://www.example.com:8080/path/path2?param=1&param2=2#fragment";
    const result = parseUrl.exec(someUrl);

    const proto = result[1];
    const host = result[3];
    const port = result[4];
    const path = result[5];
    const params = result[6];
    const fragment = result[7];

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


// Constructor function
function SampleClass2(name, age) {
    this['name'] = name;
    this['age'] = age;
}

SampleClass2.prototype.dumpAge = function() {
    debugConsole.writeln(`Name: ${this.name}, Age: ${this.age}`);
}

class ParentClass {
    constructor() {
        this.planet = "Earth";
    }

    getPlanet() {
        return this.planet;
    }
}

class BindTestClass extends ParentClass {
    constructor() {
        super();

        this.state = "Hello ";

        // https://stackoverflow.com/a/10115970/1611055
        // Bind creates a new function that will force the this inside the function to be the
        // parameter passed to bind().
        this.boundMethod = this.boundMethod.bind(this);
    }

    unboundMethod() {
        return `${this.state} ${this.getPlanet()}`;
    }

    boundMethod() {
        return `${this.state} ${this.getPlanet()}`;
    }
}


class BaseClass {
    constructor() {
        this.content = "World";
    }

    myMethod() {
        debugConsole.writeln("BaseClass::myMethod() with this=");
        debugConsole.dumpObjectFlat(this);
    }
}


class DerivedClass extends BaseClass {
    constructor() {
       super();

       this.content = "Hello";

       // Required when myMethod is called as an event handler since the event handler code seems to
       // set the calling context to something else!
       this.myMethod = this.myMethod.bind(this);
    }

    myMethod() {
        debugConsole.writeln("DerivedClass::myMethod() with this=");
        debugConsole.dumpObjectFlat(this);
    }
}

function classSampleAction() {
    debugConsole.writeln("class Sample");

	const p = new SampleClass(45, 98);
	debugConsole.writeln("Point:" + p.toString());

    const x = new SampleClass2('Peter', 43);
    x.dumpAge();

    debugConsole.writeln("\nthis Test -------------------")
    const o = new BindTestClass();
    debugConsole.writeln(`Unbound: ${o.unboundMethod()}`);
    debugConsole.writeln(`Bound  : ${o.boundMethod()}`);


    debugConsole.writeln("\nInheritance example\n=====================================");
    const cls1 = new BaseClass();
    cls1.myMethod();
    const cls2 = new DerivedClass();
    cls2.myMethod();
}


const bndeObject = new DerivedClass();

function importExportExample() {
    debugConsole.writeln("Checking imported values");
    debugConsole.writeln(`importExample: ${importExample}`)
    debugConsole.writeln(`someExport: ${someExport}`)
}

export function _initializeSamples() {
    // initializeDebugConsole();
    document.querySelector('#vads').addEventListener('click', variablesAction);
    document.querySelector('#evads').addEventListener('click', enhancedVariablesAction);
    document.querySelector('#_clearButton').addEventListener('click', clearProxy);
    document.querySelector('#doa').addEventListener('click', dumpAction);
    document.querySelector('#joa').addEventListener('click', objectAction);
    document.querySelector('#foa').addEventListener('click', functionAction);
    document.querySelector('#exa').addEventListener('click', exceptionAction);
    document.querySelector('#tda').addEventListener('click', traverseDOMAction);
    document.querySelector('#csa').addEventListener('click', colorSampleAction);
    document.querySelector('#asa').addEventListener('click', arraySampleAction);
    document.querySelector('#rsa').addEventListener('click', regexpSampleAction);
    document.querySelector('#clsa').addEventListener('click', classSampleAction);
    document.querySelector('#jqs').addEventListener('click', jquerySampleAction);
    document.querySelector('#promise').addEventListener('click', promiseExample);
    document.querySelector('#async').addEventListener('click', asyncExample);
    document.querySelector('#fpgm').addEventListener('click', functionalExamples);
    document.querySelector('#mies').addEventListener('click', importExportExample);
    document.querySelector('#yield').addEventListener('click', yieldExample);
    document.querySelector('#bnde').addEventListener('click', bndeObject.myMethod);

    initializeClock();
}
