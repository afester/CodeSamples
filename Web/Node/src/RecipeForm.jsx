import {Button, Form} from 'react-bootstrap'
import RecipeFormHeader from "./RecipeFormHeader";
import {useState} from "react";
import StarRating from "./StarRating";
// import {useSelector} from "react-redux";

export default function RecipeForm({isVisible, onClose}) {
    const [titleText, setTitleText] = useState();
    const [ingredientsText, setIngredientsText] = useState();
    const [instructionsText, setInstructionsText] = useState();
    const [rating, setRating] = useState(0);

    function submitData() {

        const ingredientsList = [{
            amount: 1,
            ingredient: ingredientsText,
            unit: 'lb'
        }]

        const payload = {
            title: titleText,
            ingredients: ingredientsList,
            instructions: instructionsText,
            rating: rating
        }

        fetch(`/api/v1/recipes`, {
                    method: 'POST',
                    body: JSON.stringify(payload),
                 }
                );
    }

    return isVisible ? <div className={'recipeDialog'}>
            <RecipeFormHeader onClose={onClose}/>
            <Form onSubmit={() => submitData()} className={'recipeForm'}>
                <Form.Group>
                    <Form.Label>Title:</Form.Label>
                    <Form.Control onChange={text => setTitleText(text.target.value)} value={titleText} type="text" placeholder="Recipe title" />
                </Form.Group>

                <Form.Group>
                    <Form.Label>Ingredients:</Form.Label>
                    <Form.Control onChange={text => setIngredientsText(text.target.value)} value={ingredientsText} type="text" placeholder="Recipe ingredients" />
                </Form.Group>

                <Form.Group>
                    <Form.Label>Instructions:</Form.Label>
                    <Form.Control onChange={text => setInstructionsText(text.target.value)} value={instructionsText} type="text" placeholder="Recipe instructions" />
                </Form.Group>

                <StarRating onChange={rating => setRating(rating)} rating={rating} totalStars={5}/>

                <Button variant="primary" type="submit">
                    Submit
                </Button>
           </Form>
        </div> : null;
}
