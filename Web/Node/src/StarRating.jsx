import React, {useEffect, useState} from "react";
import { FaStar } from "react-icons/fa";
import PropTypes from "prop-types";

const Star = ({ selected = false, selectedColor="red", unselectedColor="gray", onSelect = f => f }) => (
  <FaStar color={selected ? selectedColor : unselectedColor} onClick={onSelect} />
);

function StarRating({ rating = 0,
                                     totalStars = 5,
                                     style={}}) {
  const [currentRating, setRating] = useState(rating);

  // Remember: useEffect is called for EACH DOM update - if we simply set the initial rating again here,
  // there will be no actual change to the number of highlighted stars (since it will override the
  // value just set on the onSelect() handler below)
  // useEffect(() => {setRating(rating)});

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

// This works - note that the validation is done at runtime and errors are logged to the console
StarRating.propTypes = {
  rating: PropTypes.number.isRequired,
  totalStars: PropTypes.number.isRequired,
  style: PropTypes.object
}

export default StarRating;
