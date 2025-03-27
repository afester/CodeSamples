import React from "react";
import {createRoot} from "react-dom/client";
import {Provider} from "react-redux";
import {BrowserRouter} from "react-router-dom";            // create and configure the redux store

import RecipeApp from "./RecipeApp"
import store from "./app/store";

export function reactMain(container) {
    const root = createRoot(container);
    root.render(<Provider store={store}>
        <BrowserRouter>
            <RecipeApp/>
        </BrowserRouter>
    </Provider>);
}
