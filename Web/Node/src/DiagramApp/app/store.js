
import { configureStore } from '@reduxjs/toolkit'
import createSagaMiddleware from 'redux-saga';
import {combineReducers} from "redux";

// import {errorReducer, loginReducer, recipeReducer} from "../reducers";
// import reloadServerSaga from "../sagas/reloadServerSaga";
//
const rootReducer = combineReducers({
    // loginState : loginReducer,
    // recipes : recipeReducer,
    // errorOccurred : errorReducer,
});

// Create the Saga middleware
const sagaMiddleware = createSagaMiddleware();
const middleware = [sagaMiddleware];

// remember: default export expression; is evaluated when imported for the first time!
// Note: configureStore is the recent redux API to configure the store - createStore() is deprecated
export default configureStore({
    reducer: {rootReducer},
    middleware: (getDefaultMiddleware) =>
        getDefaultMiddleware().concat(middleware),
});

// sagaMiddleware.run(reloadServerSaga)
