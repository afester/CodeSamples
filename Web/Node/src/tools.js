/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */



function dumpObjectRec(indent, objName, obj) {
    const prefix = '                    '.substring(0, indent);

    debugConsole.writeln("\n" + prefix + "Properties of " + typeof obj + " " + objName + ":\n" + prefix + "----------------------------");

    for (let name in obj) {
        debugConsole.writeln(prefix + "  " + name + " (" + typeof name + ") own property: " + obj.hasOwnProperty(name) + " = " + obj[name]);
    }

    const ownProperties = Object.getOwnPropertyNames(obj);
    if (ownProperties.length > 0) {
        debugConsole.writeln("\n" + prefix + "  Own properties:\n" + prefix + "  -------------------------");

        for (let idx in ownProperties) {
            name = ownProperties[idx];
            debugConsole.writeln(prefix + "  " + name + " (" + typeof obj[name] + ") = " + obj[name]);

            if (obj[name] !== null && typeof obj[name] === 'object') {
                dumpObjectRec(indent + 2, name, obj[name]);
            }
        }
    }
}


export function dumpObject(objName, obj) {
    dumpObjectRec(0, objName, obj);
}

class DebugConsole {
    constructor() {
        this.textArea = document.getElementById("_textArea");
        this.textNode = document.createTextNode("JavaScript console activated.\n");
        this.textArea.appendChild(this.textNode);
    }

    appendText(text) {
        this.textNode.data = this.textNode.data + text;
        this.textArea.scrollTop = this.textArea.scrollHeight;
    }

    writeln(text) {
        if (text) {
            this.appendText(text);
        }

        this.appendText("\n");
    }

    clear() {
        this.textNode.data = "";
    }
}

const debugConsole = new DebugConsole();

function clearProxy() {
    debugConsole.clear();
}

export {debugConsole, clearProxy};
