import React from "react";
import Recipe from "./Recipe";
import {deleteRecipeAction} from "./actions";
import {useDispatch, useSelector} from "react-redux";

function RecipeList() {
    const recipeList = useSelector(state => state.rootReducer.recipes);
    const dispatch = useDispatch();

    function removeRecipe(rId) {
        fetch(`/api/v1/recipe/${rId}`, {
                method: 'DELETE',
             }).then(dispatch(deleteRecipeAction(rId)));    // remove recipe from UI

            // ).then(fetch("/api/v1/recipes")              // Reload remaining recipes from database
            // .then(response => response.json())
            // .then(jsonData => dispatch(loadRecipesAction(jsonData))));
    }

    return <div className="recipes">
               {recipeList ? recipeList.map((e, idx) => <Recipe key={idx} recipe={e} onRemove={rId => {
                   removeRecipe(rId)
               }
               }/>) : "No Data"}
           </div>;
}

export default RecipeList;
