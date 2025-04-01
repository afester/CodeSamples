/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */


export default function Box({x, y, w, h, fillColor, outlineColor, lines}) {
    const textLines = lines.map((l, idx) =>
            <text x={x} y={y + 10*idx}>{l}</text>
        );

    return <>
        <rect x={x} y={y} width={w} height={h} fill={fillColor} stroke={outlineColor} rx={3} ry={3}></rect>
        {textLines}
        </>;
}
