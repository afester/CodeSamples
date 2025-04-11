/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

import { useState } from "react";

export default function Box({ x, y, w, h, fillColor, outlineColor, lines }) {
    const [isSelected, setIsSelected] = useState(false);

    function boxClicked() {
        setIsSelected(!isSelected);
    }

    const textLines = lines.map((l, idx) => (
        <text x={x} y={y + 10 * (idx + 1)}>
            {l}
        </text>
    ));

    if (isSelected) {
        outlineColor = "red";
    }

    return (
        <>
            <rect
                onClick={boxClicked}
                x={x}
                y={y}
                width={w}
                height={h}
                fill={fillColor}
                stroke={outlineColor}
                rx={3}
                ry={3}
            ></rect>
            {textLines}
        </>
    );
}
