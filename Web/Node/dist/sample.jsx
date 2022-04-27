
function getReceipes() {
    return [
        {
        title: "Baked Salmon",
        ingredients: [
            {amount: 2, unit: "lb", ingredient: "Salmon"},
            {amount: 5, unit: "sprigs", ingredient: "fresh Rosemary"},
            {amount: 2, unit: "tablespoons", ingredient: "Olive oil"},
            {amount: 2, unit: "small", ingredient: "lemons"},
            {amount: 1, unit: "teaspoon", ingredient: "kosher salt"},
            {amount: 4, unit: "cloves of", ingredient: "chopped garlic"},
        ],
        instructions: "Preheat the oven to 375 degrees. Lightly coat aluminum foil with oil. Place salmon on foil. " +
            "Cover with rosemary, sliced lemons, chopped garlic. Bake for 15-20 minutes until cooked through. Remove from oven.",
        },
        {
        title: "Chicken Noodle Soup",
        ingredients: [
            {amount: 2, unit: "lb", ingredient: "salmon"},
            {amount: 5, unit: "sprigs", ingredient: "fresh rosemary"},
            {amount: 2, unit: "tablespoons", ingredient: "olive oil"},
            {amount: 2, unit: "small", ingredient: "Lemons"},
            {amount: 1, unit: "teaspoon", ingredient: "kosher Salt"},
            {amount: 4, unit: "cloves of", ingredient: "chopped garlic"},
        ],
        instructions: "Chop and saute onion",
        },
    ]
}

function IngredientsList(props) {
    return <>
               <h3>Ingredients:</h3>
               <ul>
                   {props.items.map((e, idx) => <li key={idx}>{e.amount} {e.unit} {e.ingredient}</li>)}
               </ul>
           </>;
}

function Receipe({receipe}) {
    return <div>
             <h1>{receipe.title}</h1>
             <IngredientsList items={receipe.ingredients} />
             <div>{receipe.instructions}</div>
           </div>;
}

function ReceipeList(props) {
    return <div className="receipes">
               {props.items.map((e, idx) => <Receipe key={idx} receipe={e} />)}
           </div>;
}

function main() {
    ReactDOM.render(
        <ReceipeList items={getReceipes()} />,
        document.getElementById("root")
    );
}

main();
