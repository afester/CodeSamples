/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

import { FadeLoader } from "react-spinners";
import Box from "./Box";
import Circle from "./Circle";
import TextBox from "./TextBox";
import Line from "./Line";
import { useDispatch, useSelector, connect } from "react-redux";
import { loadDiagram } from "../actions";
import { selectNode } from "../actions/diagramActions";

const getSelectedNodes = (state) => {
    return state.rootReducer.get("diagramData").get("selectedNodes");
};

const getDiagramData = (state) => {
    return state.rootReducer.get("diagramData");
};

function DiagramArea({ width, height, diagramData, selectedNodes}) {
    const dispatch = useDispatch();

    console.log(`Data: ${diagramData}`);
    if (!diagramData.get("nodes")) {
        dispatch(loadDiagram());
        return <FadeLoader />;
    }

    const nodeSelected = (key, nodeData, selected, ctrlKey) => {
        console.log(`NODE SELECTED: ${key} ${selected} ${ctrlKey}`);
        dispatch(selectNode(key, nodeData, selected, ctrlKey));
    }

    const nodes = diagramData.get("nodes").map((v, idx) => {
        switch (v.get("type")) {
            case "Box":
                const key = `box-${idx}`;
                return (
                    <Box
                        x={v.get("x")}
                        y={v.get("y")}
                        w={v.get("w")}
                        h={v.get("h")}
                        fillColor={"#fff1c1"}
                        outlineColor={"#3d3d3d"}
                        key={key}
                        selectBox={(selected, ctrlKey) => nodeSelected(key, v, selected, ctrlKey)}
                        isSelected={selectedNodes?.has(key)}
                    />
                );

            case "TextBox":
                const tkey = `textbox-${idx}`;
                return (
                    <TextBox
                        x={v.get("x")}
                        y={v.get("y")}
                        lines={v.get("lines")}
                        fillColor={"#fff1c1"}
                        outlineColor={"#3d3d3d"}
                        key={tkey}
                        selectBox={(selected, ctrlKey) => nodeSelected(tkey, v, selected, ctrlKey)}
                        isSelected={selectedNodes?.has(tkey)}
                    />
                );

            case "Circle":
                return (
                    <Circle
                        x={v.get("x")}
                        y={v.get("y")}
                        r={v.get("r")}
                        fillColor={"#fff1c1"}
                        outlineColor={"#3d3d3d"}
                        key={`circle-${idx}`}
                    />
                );

        }
    });

    const edges = diagramData.get("edges").map((v, idx) => {
        return (
            <Line
                x1={v.get("x1")}
                y1={v.get("y1")}
                x2={v.get("x2")}
                y2={v.get("y2")}
                color={v.get("strokeColor")}
                width={v.get("strokeWidth")}
                key={`line-${idx}`}
            />
        );
    });

    return (
        <div
            style={{
                background: "white",
                width: `${width}px`,
                height: `${height}px`,
            }}
        >
            <svg width={width} height={height}>
                {edges}
                {nodes}
            </svg>
        </div>
    );
}

function mapStateToProps(state) {
    return {
        diagramData: getDiagramData(state),
        selectedNodes: getSelectedNodes(state),
    };
}

export default connect(mapStateToProps)(DiagramArea);
