/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

import { useState } from "react";

export default function Circle({
    x,
    y,
    r,
    fillColor,
    outlineColor,
    innerComponent,
}) {
    const [isSelected, setIsSelected] = useState(false);

    function boxClicked() {
        setIsSelected(!isSelected);
    }

    if (isSelected) {
        outlineColor = "red";
    }

    return (
        <g onClick={boxClicked}>
            <circle
                cx={x}
                cy={y}
                r={r}
                fill={fillColor}
                stroke={outlineColor}
            />
            {innerComponent}
        </g>
    );
}
