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
}

