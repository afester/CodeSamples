import React from "react";
import Immutable from "immutable";
import installImmutableDevTools from "immutable-devtools";
import { createRoot } from "react-dom/client";
import { Provider } from "react-redux";
import { BrowserRouter } from "react-router-dom"; // create and configure the redux store

import DiagramApp from "./components/DiagramApp";
import store from "./app/store";

export function diagrammerMain(container) {
    installImmutableDevTools(Immutable);
    const root = createRoot(container);
    root.render(
        <Provider store={store}>
            <BrowserRouter>
                <DiagramApp />
            </BrowserRouter>
        </Provider>,
    );
}
