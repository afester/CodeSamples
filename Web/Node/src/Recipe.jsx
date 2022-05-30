import React, {useState} from "react";
import {createRoot} from "react-dom/client";
import StarRating from "./StarRating";
import recipesData from "./recipes.json";
import { FaTrash } from "react-icons/fa";
import PropTypes from 'prop-types';

function IngredientsList(props) {
    return <>
               <h3>Ingredients:</h3>
               <ul>
                   {props.items.map((e, idx) => <li key={idx}>{e.amount} {e.unit} {e.ingredient}</li>)}
               </ul>
           </>;
}

function Receipe({recipe, onRemove = f => f}) {
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

function ReceipeList({items}) {
    const  [receipes, setReceipes] = useState(items)

    return <div className="recipes">
               {receipes.map((e, idx) => <Receipe key={idx} recipe={e} onRemove={rId =>
                        {
                            const newItems = receipes.filter(receipe => receipe.id !== rId);
                            setReceipes(newItems);
                        }
                    }/>)}
           </div>;
}

ReceipeList.propTypes = {
    xyz: PropTypes.string
}





function main() {
    const container = document.getElementById('root');
    const root = createRoot(container);
    root.render(<ReceipeList items={recipesData} />);
}

main();
