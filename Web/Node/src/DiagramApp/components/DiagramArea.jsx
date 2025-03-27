/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */

export default function DiagramArea() {
    return <div style={{"background": "white",
                        "width": "800px",
                        "height": "600px"}}>
        <svg>
            <rect x={20} y={20} width={50} height={50} fill={"green"}></rect>
            <rect x={120} y={120} width={50} height={50} fill={"red"}></rect>
        </svg>
    </div>;
}
