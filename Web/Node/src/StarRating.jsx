import React, {useState} from "react";
import { FaStar } from "react-icons/fa";

const Star = ({ selected = false, selectedColor="red", unselectedColor="gray", onSelect = f => f }) => (
  <FaStar color={selected ? selectedColor : unselectedColor} onClick={onSelect} />
);

export default function StarRating({ totalStars = 5,
                                     style={}}) {
  const [currentRating, setRating] = useState(3);

  const result = new Array(totalStars);
  for (let idx = 0; idx < totalStars; idx++) {
    result[idx] = <Star key={idx}
                        selected={currentRating > idx}
                        onSelect={() => setRating(idx + 1)}
                        selectedColor={style.selectedColor}
                        unselectedColor={style.unselectedColor}/>;
  }

  return style.backgroundColor ? <div style={{backgroundColor: style.backgroundColor}}>{result}</div>
                               : <div>{result}</div>
}
