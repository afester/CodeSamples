/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

var textArea, textNode;

export var debugConsole = {
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

// debugConsole.initialize();
debugConsole["initialize"]();
