/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

import { useState, useRef, useEffect } from "react";
import Box from "./Box";

const TEXT_PADDING = 5; // Space between box border and contained text (pixel)

/**
 *
 * @param x
 * @param y
 * @param w
 * @param h
 * @param fillColor
 * @param outlineColor
 * @param lines
 * @returns {JSX.Element}
 * @constructor
 */
export default function TextBox({
    x,
    y,
    fillColor,
    outlineColor,
    lines,
    selectBox,
    isSelected,
}) {
    const [boxLayout, setBoxLayout] = useState();
    const textGroupRef = useRef();

    useEffect(() => {
        const textSize = textGroupRef.current.getBBox();
        console.log(`TextBox: ${textSize.width}x${textSize.width}`);

        const boxLayout = {
            lineHeight: textSize.height, // Maximum height of one text line
            width: textSize.width,
            height: textSize.height,
        };

        setBoxLayout(boxLayout);
    }, []);

    if (boxLayout) {
        console.log(`BoxLayout: ${JSON.stringify(boxLayout)}`);

        let lineCount = 0;
        const textLines = (
            <text key={1243}>
                {lines.map((line, idx2) => {
                    // <text x={x} y={y + 10 * (idx + 1)}>
                    lineCount++;
                    return (
                        <tspan
                            x={x + TEXT_PADDING}
                            y={y + boxLayout.height * (idx2 + 1)}
                            key={idx2}
                        >
                            {line}
                        </tspan>
                    );
                })}
            </text>
        );

        return (
            <Box
                x={x}
                y={y}
                w={boxLayout.width + 2 * TEXT_PADDING}
                h={boxLayout.height * lineCount + TEXT_PADDING}
                fillColor={fillColor}
                outlineColor={outlineColor}
                innerComponent={textLines}
                selectBox={selectBox}
                isSelected={isSelected}
            ></Box>
        );
    } else {
        // calculate text size on first render
        const textLines = (
            <text key={1243}>
                {lines.map((line, idx2) => (
                    <tspan x={0} y={0} key={idx2}>
                        {line}
                    </tspan>
                ))}
            </text>
        );

        return (
            <g
                ref={(r) => {
                    textGroupRef.current = r;
                }}
            >
                {textLines}
            </g>
        );
    }
}
