/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */



if (typeof Object.create !== 'function') {
     Object.create = function (o) {
         var F = function () {};
         F.prototype = o;
         return new F();
     };
}

var debugConsole = {
      "initialize" :
         function () {
			   textArea = document.getElementById("_textArea");
			   textNode = document.createTextNode("JavaScript console activated.\n");
			   textArea.appendChild(textNode);
		   },

		"appendText" :
		   function (text) {
			   textNode.data = textNode.data + text;
            textArea.scrollTop = textArea.scrollHeight;
		   },

	    "writeln" :
	         function (text) {
	            this.appendText(text + "\n");
	         },

		"clear" : 
		   function () {
            textNode.data = "";
		   }
};

var Point = {
      "x" : 0,

      "y" : 0,

      "toString" :
         function() {
            return "Point[" + this.x + ", " + this.y + "]";
         }
};


function dumpObjectRec(indent, objName, obj) {
   var prefix = '                    '.substring(0, indent);
   debugConsole.writeln("\n" + prefix + "Properties of " + typeof obj + " " + objName + ":\n" + prefix + "----------------------------");

   for (var name in obj) {
      debugConsole.writeln(prefix + "  " + name + " (" + typeof name + ") own property: " + obj.hasOwnProperty(name) + " = " + obj[name]);      
   }

   var ownProperties = Object.getOwnPropertyNames(obj);
   if (ownProperties.length > 0) {
      debugConsole.writeln("\n" + prefix + "  Own properties:\n" + prefix + "  -------------------------");
      
      for (var idx in ownProperties) {
         var name = ownProperties[idx];
         debugConsole.writeln(prefix + "  " + name + " (" + typeof obj[name] + ") = " + obj[name]);
   
         if (obj[name] != null && typeof obj[name] == 'object') {
            dumpObjectRec(indent + 2, name, obj[name]);
         }
      }
   }
}


function dumpObject(objName, obj) {
   dumpObjectRec(0, objName, obj);
}


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


function functionAction() {
   var add = function (a, b) {
      return a + b;
   };
   
   // add is a reference to a function object, so it can be invoked
   var result = add(5, 7);

   debugConsole.writeln("5 + 7 = " + result);
   
   /** Method Invocation Pattern - "this" refers to the current object **/ 
   var someObject = {
         'value' : 'SomeValue',

         'invokeMethod' : function() {
            debugConsole.writeln(this.value + "(" + typeof this.value + ")");
         }
   };
   someObject.invokeMethod();

   var someOtherObject = {
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
      var that = this;

      var someFunction2 = function () {
         that.value = 1.8;
      }; 
      
      someFunction2();
   };
   someOtherObject.setDefaultValue2();
   debugConsole.writeln("someOtherObject.value: " + someOtherObject.value); // Now correctly 1.8
   debugConsole.writeln("global value: " + value);                          // global property is still 1.5
   
   /** Constructor Invocation Pattern */
   var SomeValue = function (aValue) {    // constructor function
      this.value = aValue;
   };
   
   SomeValue.prototype.getValue = function() {
      return this.value;
   };
   
   var value1 = new SomeValue("Hello");
   var value2 = new SomeValue("World");
   debugConsole.writeln("value1: " + value1.getValue());   
   debugConsole.writeln("value2: " + value2.getValue());
   
   /** Apply invocation pattern */
   // each function has the "apply" method which can be used to invoke the function.
   // the first parameter is the object which will be passed to the function 
   // as "this" parameter, the optional second parameter are the parameters passed to the function.
   var result = SomeValue.prototype.getValue.apply(value1);
   debugConsole.writeln("value1 through apply(): " + result);
   
   
   /** The arguments parameter - allow access to all parameters */
   var calculateAvg = function() {
      var sum = 0;
      for (var idx = 0;  idx < arguments.length;  idx += 1) {
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
      var sum = 0;
      for (var idx = 0;  idx < arguments.length;  idx += 1) {
         sum += arguments[idx];
      }
      return sum / arguments.length;
   };

   debugConsole.writeln(Math.avg(1, 3, 4, 5));
}


function exceptionAction() {
   var calculateAvg = function() {
      if (arguments.length == 0) {
         throw {
            name : 'DivByZero',
            message: 'Number of parameters is 0'
         };
      }

      var sum = 0;
      for (var idx = 0;  idx < arguments.length;  idx += 1) {
         sum += arguments[idx];
      }
      return sum / arguments.length;
   };
   
   try {
      debugConsole.writeln("Average: " + calculateAvg());
   } catch(e) {
      dumpObject('e', e);
   }

   var count = 0;
   try {
      var f1 = function f2() {
         count++;
         f2();
      };
      f1();
   } catch(e) {
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

   var fade = function (node) {
      var level = 1;

      var step = function () {
          var hex = level.toString(16);
          node.style.backgroundColor = '#' + hex + hex + hex; // FFFF' + hex + hex;
          if (level < 15) {
              level += 1;
              setTimeout(step, 100);
          }
      };
      setTimeout(step, 100);
  };

  var coloredNode = document.getElementById('_textArea');
  fade(coloredNode);
}


function arraySampleAction() {
   var a = [1, 2, 3];
   dumpObject("a", a);
   
   a[1] = a[2];
   dumpObject("a", a);
   
  
   var a1 = [ {name: "first"}, {name: "second"}, {name: "third"}];
   dumpObject("a1", a1);
   a1[1] = a1[2];
   dumpObject("a1", a1);


   var a2 = [ {name: "first"}, {name: "second"}, {name: "third"}, {name: "fourth"}];
   for (var i = 1;  i < a2.length - 1;  i++) {
      a2[i] = a2[i+1];
   }
   dumpObject("a2", a2);
   
   
   var someArray = ['a', 'b', 'c'];
   debugConsole.writeln('Array length:' + someArray.length);
   someArray[5] = 'e';
   debugConsole.writeln('Array length:' + someArray.length + ',' + someArray[3]);
   debugConsole.writeln('=========================');

   var someArray2 = ['a', 'b', 'c', 'd', 'e'];
   debugConsole.writeln('someArray2: ' + someArray2);
   
   var someArray3 = someArray2.slice(1, 3);
   debugConsole.writeln('someArray2.slice(1, 3): ' + someArray3);

   debugConsole.writeln('someArray2: ' + someArray2);

   var someArray4 = someArray2.splice(2, 1);
   debugConsole.writeln('someArray2.splice(2, 1): ' + someArray4);
   debugConsole.writeln('someArray2: ' + someArray2 + '(' + someArray2.length + ')');
}
