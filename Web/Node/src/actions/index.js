

// The type field should be a string that gives this action a descriptive name, like "todos/todoAdded".
// We usually write that type string like "domain/eventName", where the first part is the feature or category
// that this action belongs to, and the second part is the specific thing that happened.
//
// An action object can have other fields with additional information about what happened.
// By convention, we put that information in a field called payload.
export const loginAction = () => {
    return {
        type: 'LOGIN'
    }
}
export const logoutAction = () => {
    return {
        type: 'LOGOUT'
    }
}


export const loadRecipesAction = (recipes) => {
    return {
        type: 'LOAD',
        payload: recipes
    }
}

export const reloadServerAction = () => {
    return {
        type: 'RELOAD_SERVER',
    }
}


export const loadRecipesFailedAction = (errorDesc) => {
    return {
        type: 'LOAD_FAILED',
        payload: errorDesc
    }
}

export const closeErrorPanel = () => {
    return {
        type: 'CLOSE_ERROR_PANEL',
    }
}

export const deleteRecipeAction = (recipeId) => {
    return {
        type: 'DELETE_RECIPE',
        payload: recipeId
    }
}
