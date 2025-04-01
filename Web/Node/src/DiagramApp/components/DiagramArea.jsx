/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

import Box from "./Box";
import Line from "./Line";

export default function DiagramArea({width, height}) {
    return <div style={{"background": "white",
                        "width": `${width}px`,
                        "height": `${height}px`}}>
        <svg width={width} height={height}>
            <Box x={20} y={20} w={50} h={50} fillColor={"#fff1c1"} outlineColor={"#3d3d3d"}
                 lines={["Hello", "World"]} />
            <Box x={120} y={120} w={50} h={50} fillColor={"#fff1c1"} outlineColor={"#3d3d3d"}
                 lines={["Hello", "World"]} />
            <Line x1={70} y1={70} x2={120} y2={120} color={"blue"} />
        </svg>
    </div>;
}
