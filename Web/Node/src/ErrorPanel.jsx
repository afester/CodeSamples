import {Button} from 'react-bootstrap'
import {FaTimes} from "react-icons/fa";
import {useDispatch} from "react-redux";
import {closeErrorPanel} from "./actions";

function ErrorPanel({message}) {
    const dispatch = useDispatch();

    function closeDialog() {
        dispatch(closeErrorPanel());
    }

    return <div className="errorPanel">
                <span>{message}</span>
                <Button className={'errorButtonClass'}
                        onClick={() => closeDialog()}><FaTimes /></Button>
           </div>;
}

export default ErrorPanel;
