/**
 * This work is licensed under the Creative Commons Attribution 3.0 Unported
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by/3.0/ or send a letter to Creative
 * Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 */


export default function Line({x1, y1, x2, y2, color}) {
    return <line x1={x1} y1={y1} x2={x2} y2={y2} stroke={color}></line>;
}
