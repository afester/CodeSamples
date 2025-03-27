/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

import {reactMain} from "./RecipeApp"
import {_initializeSamples} from "./JSSamples/jsSamples";
import {diagrammerMain} from "./DiagramApp"

function main() {
    const container = document.getElementById('root');
    if (container) {
        // Render the React page
        reactMain(container);
    } else {
        const diagrammer = document.getElementById('diagrammer');
        if (diagrammer) {
            diagrammerMain(diagrammer);
        } else {
            // Initialize the plain old javascript page
            _initializeSamples();
        }
    }
}

main();
