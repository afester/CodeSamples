
import $ from "jquery";

function getReceipes() {
    return [
        {
        title: "Baked Salmon",
        ingredients: [
            {amount: 2, unit: "lb", ingredient: "Salmon"},
            {amount: 5, unit: "sprigs", ingredient: "fresh Rosemary"},
            {amount: 2, unit: "tablespoons", ingredient: "olive oil"},
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
            {amount: 2, unit: "small", ingredient: "lemons"},
            {amount: 1, unit: "teaspoon", ingredient: "kosher salt"},
            {amount: 4, unit: "cloves of", ingredient: "chopped garlic"},
        ],
        instructions: "Chop and saute onion",
        },
    ]
}


function IngredientsList(props) {
    return React.createElement(
      "ul",
      { className: "ingredients" },
        props.items.map((e, idx) =>
            React.createElement("li", {key: idx}, `${e.amount} ${e.unit} ${e.ingredient}`)));
}


function Receipe(props) {
    return React.createElement(
        "div",
        null,

        React.createElement(
        "h1",
        null, props.receipe.title),

        React.createElement(IngredientsList, { items: props.receipe.ingredients }, null),

        React.createElement(
        "div",
        null, props.receipe.instructions),
    );
}

function reactSampleMain() {
    const receipeViews = getReceipes().map(r => React.createElement(Receipe, {receipe: r} ));
    ReactDOM.render(
        receipeViews,
        document.getElementById("root")
    );
}

// reactSampleMain();
