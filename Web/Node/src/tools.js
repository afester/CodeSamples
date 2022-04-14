/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */


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
