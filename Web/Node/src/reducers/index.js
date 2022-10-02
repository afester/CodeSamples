/**
 * A reducer changes the state of the store according to an action.
 * Note that the reducer is initially called with an "@@INIT" action - by simply returning the
 * state parameter in the default branch below, we can set the initial value for the state by
 * setting the initial value in the parameter list.
 *
 * @param state  The current state of the value
 * @param loginAction The action which is performed
 * @returns {object} The new state depending on the action
 */
export const loginReducer = (state= false, loginAction) => {
    switch(loginAction.type) {
        case 'LOGIN' :
            return true;

        case 'LOGOUT' :
            return false;

        default :
            return state;
     }
}

// A reducer is a function that receives the current state and an action object, decides how to update the
// state if necessary, and returns the new state: (state, action) => newState. You can think of a reducer as
// an event listener which handles events based on the received action (event) type.
//
// INFO
// "Reducer" functions get their name because they're similar to the kind of callback function you pass to
// the Array.reduce() method.
// Reducers act like event listeners, and when they hear an action they are interested in, they update the
// state in response.
export const recipeReducer = (state = [], recipeAction) => {
    switch(recipeAction.type) {
        case 'ADD_RECIPE' : return state;      // TODO

        case 'DELETE_RECIPE' :
            return state.filter(state => state.id !== recipeAction.payload);

        case 'LOAD' :
            return recipeAction.payload;

        default :
            return state;
     }
}

export const errorReducer = (state = null, errorAction) =>  {
    switch (errorAction.type) {
        case 'LOAD_FAILED':
            return 'Could not load recipes: ' + errorAction.payload.message;

        case 'CLOSE_ERROR_PANEL':
            return null;

        default:
            return state;
    }
}
