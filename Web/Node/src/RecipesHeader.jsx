
import {Button} from 'react-bootstrap'
import {FaPlus, FaSync, FaWalking, FaHome} from "react-icons/fa";
import {useNavigate} from "react-router-dom";

export default function RecipesHeader({onAddRecipe, onRefresh, onHome, onLogout}) {
    const navigate = useNavigate();

    return <div>
              <h1>Recipes Management</h1>
              <Button className={'buttonClass'}
                        onClick={onLogout}>
                  <FaWalking/>
              </Button>
              <Button className={'buttonClass'}
                        onClick={() => {navigate("/"); navigate(0)}}>
                  <FaHome/>
              </Button>
              <Button className={'buttonClass'}
                        onClick={onAddRecipe}>
                  <FaPlus/>
              </Button>
              <Button className={'buttonClass'}
                        onClick={onRefresh}>
                  <FaSync/>
              </Button>
           </div>
}
