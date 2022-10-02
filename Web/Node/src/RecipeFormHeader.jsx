
import {Button} from 'react-bootstrap'
import {FaTimes} from "react-icons/fa";

export default function RecipeFormHeader({onClose}) {
    return <div>
               <Button onClick={onClose}><FaTimes/></Button>
           </div>
}
