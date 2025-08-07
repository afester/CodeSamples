
import { configureStore } from '@reduxjs/toolkit'
import createSagaMiddleware from 'redux-saga';
import {combineReducers} from "redux-immutable";
import { diagramReducer } from "../reducers";
import Immutable from "immutable";

// import {errorReducer, loginReducer, recipeReducer} from "../reducers";
// import reloadServerSaga from "../sagas/reloadServerSaga";
//
const rootReducer = combineReducers({
    diagramData: diagramReducer
    // loginState : loginReducer,
    // recipes : recipeReducer,
    // errorOccurred : errorReducer,
});

// Create the Saga middleware
const sagaMiddleware = createSagaMiddleware();
const middleware = [sagaMiddleware];

// Note: configureStore is the recent redux API to configure the store - createStore() is deprecated
// See https://redux.js.org/introduction/why-rtk-is-redux-today
// remember: default export expression; is evaluated when imported for the first time!
export default configureStore({
    reducer: {rootReducer},
    middleware: (getDefaultMiddleware) =>
        getDefaultMiddleware().concat(middleware),
});

// sagaMiddleware.run(reloadServerSaga)
