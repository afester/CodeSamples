/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

import {createRoot} from "react-dom/client";
import RecipeApp from "./RecipeApp"
import React from "react";
import {_initializeSamples} from "./jsSamples";
import {Provider} from "react-redux";
import store from "./app/store";
import {BrowserRouter} from "react-router-dom";            // create and configure the redux store

function reactMain(container) {
    const root = createRoot(container);
    root.render(<Provider store={store}>
        <BrowserRouter>
                    <RecipeApp />
        </BrowserRouter>
                </Provider>);
}

function main() {
    const container = document.getElementById('root');
    if (container) {
        // Render the React page
        reactMain(container);
    } else {
        // Initialize the plain old javascript page
        _initializeSamples();
    }
}

main();
