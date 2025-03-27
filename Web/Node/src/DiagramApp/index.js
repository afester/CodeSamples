import React from "react";
import {createRoot} from "react-dom/client";
import {Provider} from "react-redux";
import {BrowserRouter} from "react-router-dom";            // create and configure the redux store

import DiagramArea from "./components/DiagramArea";
import store from "./app/store";

export function diagrammerMain(container) {
    const root = createRoot(container);
    root.render(<Provider store={store}>
        <BrowserRouter>
            <DiagramArea width={800} height={600} />
        </BrowserRouter>
    </Provider>);
}
