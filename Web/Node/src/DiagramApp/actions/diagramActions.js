/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

import {ACTIONS} from '../constants';

export function loadDiagram() {
    console.log("ACTION: loadDiagram()");
    return {type: ACTIONS.LOAD_DIAGRAM};
}

export function selectNode(key, nodeData, selected, ctrlKey) {
    console.log(`ACTION: selectNode(${key}, ${selected})`);
    return {type: ACTIONS.SELECT_NODE,
            payload: {key, nodeData, selected, ctrlKey}
    };
}
