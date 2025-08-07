/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

import React from "react";
import {connect} from "react-redux";

// TODO: Move into separate file
const getSelectedNodes = (state) => {
    return state.rootReducer.get("diagramData").get("selectedNodes");
};


const getDiagramData = (state) => {
    return state.rootReducer.get("diagramData");
};

function PropertyPanel({fields, selectedNodes, diagramData}) {

    const fieldControls = selectedNodes && selectedNodes.size === 1 ?
        fields.map((v) => {
            return <div>{JSON.stringify(v)}</div>
        }) : null;

    const nodeData = selectedNodes && selectedNodes.size === 1
        ? diagramData.get(selectedNodes.first())
        : null;

    return (
        <div>
            <span>Property Panel</span>
            <div>{JSON.stringify(nodeData)}</div>
            {fieldControls}
        </div>
    );
}

function mapStateToProps(state) {
    return {
        selectedNodes: getSelectedNodes(state),
        diagramData: getDiagramData(state)
    };
}

export default connect(mapStateToProps)(PropertyPanel);
