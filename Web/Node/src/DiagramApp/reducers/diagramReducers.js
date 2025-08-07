import Immutable from "immutable";
import { ACTIONS } from "../constants";

export const diagramReducer = (state = Immutable.Map(), action) => {
    const { type, payload } = action;
    console.log(`diagramReducer: ${JSON.stringify(action)}`);

    switch (type) {
        case ACTIONS.LOAD_DIAGRAM:
            const data = Immutable.Map({
                nodes: Immutable.fromJS([
                    {
                        type: "TextBox",
                        x: 20,
                        y: 20,
                        fillColor: "#fff1c1",
                        outlineColor: "#3d3d3d",
                        lines: ["This is a long line", "Hello Moon"],
                    },
                    {
                        type: "TextBox",
                        x: 120,
                        y: 120,
                        fillColor: "#fff1c1",
                        outlineColor: "#3d3d3d",
                        lines: ["Hello Mars", "and", "Moon"],
                    },
                    {
                        type: "Box",
                        x: 220,
                        y: 220,
                        w: 100,
                        h: 50,
                        fillColor: "#fff1c1",
                        outlineColor: "#3d3d3d",
                    },
                    {
                        type: "Circle",
                        x: 220,
                        y: 50,
                        r: 30,
                        fillColor: "#55ff55",
                        outlineColor: "#3d3d3d",
                    },
                ]),
                edges: Immutable.fromJS([
                    {
                        x1: 70,
                        y1: 40,
                        x2: 130,
                        y2: 130,
                        strokeColor: "blue",
                        strokeWidth: 5,
                    },
                ]),
            });
            return state.merge(data);

        case ACTIONS.SELECT_NODE:
            const { key, nodeData, selected, ctrlKey } = payload;
            const selectedNodes = ctrlKey ?
                selected ? state.getIn(["selectedNodes"], Immutable.Set()).add(key)
                         : state.getIn(["selectedNodes"], Immutable.Set()).delete(key)
                : selected ? Immutable.Set().add(key) : Immutable.Set();

            const selectionData = Immutable.Map({
                selectedNodes
            });
            return state.merge(selectionData);
    }
    return state;
};
