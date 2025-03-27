import React from "react";
import StarRating from "./StarRating";
import { FaTrash } from "react-icons/fa";

function IngredientsList(props) {
    return <>
               <h3>Ingredients:</h3>
               <ul>
                   {props.items.map((e, idx) => <li key={idx}>{e.amount} {e.unit} {e.ingredient}</li>)}
               </ul>
           </>;
}

function Receipe({recipe, onRemove = f => f}) {
    console.log(recipe);

    return <div>
             <h1>{recipe.title}
             <button onClick={() => onRemove(recipe.id)}>
                <FaTrash />
             </button>
             </h1>
             <p>Rating:</p>
              <StarRating rating={recipe.rating}
                         totalStars={5}
                         style={{// backgroundColor: 'gray',
                                 selectedColor: 'orange',
                                 unselectedColor: 'lightgray'}} />
             <IngredientsList items={recipe.ingredients} />
             <div>{recipe.instructions}</div>
           </div>;
}

export default Receipe;