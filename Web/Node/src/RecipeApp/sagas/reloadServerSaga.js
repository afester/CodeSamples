
import { put, takeLatest } from 'redux-saga/effects'


// worker Saga: will be fired on USER_FETCH_REQUESTED actions
function* reloadServer(action) {
             // fetch("/api/v1/recipes?reload")
        //     .then(response => response.json())
        //     .then(jsonData => dispatch(loadRecipesAction(jsonData)));

   try {
      const response = yield fetch("/api/v1/recipes?reload");
      const data = yield response.json();
      yield put({type: "LOAD", payload: data});
   } catch (e) {
      // yield put({type: "USER_FETCH_FAILED", message: e.message});
      alert("ERROR!");
   }
}

function* reloadServerSaga() {
  yield takeLatest("RELOAD_SERVER", reloadServer);
}

export default reloadServerSaga;
