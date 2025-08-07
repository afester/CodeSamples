/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

import DiagramArea from "./DiagramArea";
import React from "react";
import PropertyPanel from "./PropertyPanel";
import Immutable from "immutable";

export default function DiagramApp() {
    const values = Immutable.fromJS([
        { type: "bool", label: "Checkbox", value: true },
        { type: "int", label: "Number", value: 4 },
        { type: "float", label: "Float", value: 3.14 },
        { type: "string", label: "String", value: "Hello World" },
    ]);

    return (
        <div>
            <div style={{ float: "left" }}>
                <DiagramArea width={800} height={600} />
            </div>
            <div style={{ background: "lightgray" }}>
                <PropertyPanel fields={values} />
            </div>
        </div>
    );
}
