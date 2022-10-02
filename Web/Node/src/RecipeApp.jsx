import RecipeList from "./RecipeList";
import RecipesHeader from "./RecipesHeader"
import RecipeForm from "./RecipeForm"
import LoginMask from "./LoginMask";
import {useState, useEffect} from "react";
import {useDispatch, useSelector} from "react-redux";
import {loadRecipesAction, loadRecipesFailedAction, logoutAction, reloadServerAction} from "./actions";
import ErrorPanel from "./ErrorPanel";


function RecipeApp(props) {
    const [showAddForm, setShowAddForm] = useState(false);
    const loggedIn = true; // useSelector(state => state.rootReducer.loginState);
    const errorOccurred = useSelector(state => state.rootReducer.errorOccurred);
    const dispatch = useDispatch();


    async function fetchDataThunk(dispatch, getState) {
        try {
            const response = await fetch("/api/v1/recipes");
            const jsonData = await response.json();
            dispatch(loadRecipesAction(jsonData));
        } catch(error) {
            dispatch(loadRecipesFailedAction(error));
        }

        // fetch("/api/v1/recipes")
        //     .then(response => response.json())
        //     .then(jsonData => dispatch(loadRecipesAction(jsonData)));
    }


    useEffect(() => {
        dispatch(fetchDataThunk);   // requires an "async function f(dispatch, getState)"!

        // fetch("/api/v1/recipes")
        //     .then(response => response.json())
        //     .then(jsonData => dispatch(loadRecipesAction(jsonData)));
    }, [dispatch]);     // ?????????

    function refreshData() {
        dispatch(reloadServerAction());

        // fetch("/api/v1/recipes?reload")
        //     .then(response => response.json())
        //     .then(jsonData => dispatch(loadRecipesAction(jsonData)));
    }

    return <div>
                {/* NOTE: for Demonstration purposes only - DO NOT IMPLEMENT LOGIN LOGIC ON THE CLIENT SIDE! */}
                {loggedIn ?
                    <div className={'appContainer'}>
                        <div className={'contentBlock'}>
                            { errorOccurred ? <ErrorPanel message={errorOccurred} /> : null }
                        </div>
                        <div className={'contentBlock'}>
                            <RecipesHeader onAddRecipe={() => setShowAddForm(true)}
                                           onRefresh={() => refreshData()}
                                           onHome={f => f}
                                           onLogout={() => dispatch(logoutAction())}/>
                            <RecipeForm isVisible={showAddForm} onClose={() => setShowAddForm(false)}/>
                            <RecipeList />
                         </div>
                    </div>
                    : <LoginMask />
                }
           </div>
}

export default RecipeApp;
