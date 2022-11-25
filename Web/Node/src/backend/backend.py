import json
import morepath


class App(morepath.App):
    pass


class Person(object):
    def __init__(self, firstname, lastname):
        self.firstname = firstname
        self.lastname = lastname

    def __str__(self):
        return "Person[firstname={}, lastname={}]".format(self.firstname, self.lastname)


class Recipe(object):
    def __init__(self, jsonData):
        print(jsonData)
        self.id = jsonData['id']
        self.title = jsonData['title']
        self.rating = int(jsonData['rating'])
        self.ingredients = jsonData['ingredients']
        self.instructions = jsonData['instructions']
        print("=====> {}".format(self.ingredients))

    def toJson(self):
        return dict(id=self.id,
                    title=self.title,
                    rating=self.rating,
                    ingredients=self.ingredients,
                    instructions=self.instructions)

    def __str__(self):
        return "Recipe[id={}, title={}]".format(self.id, self.title)

    def __repr__(self):
        return "Recipe[id={}, title={}]".format(self.id, self.title)


class RecipeCollection(object):
    def __init__(self):
        self.recipes = list()

    def reload(self):
        self.recipes = list()
        with open("recipes.json", "r") as reader:
            recipesData = json.load(reader)
            for recipeData in recipesData:
                recipe = Recipe(recipeData)
                recipes.append(recipe)

    def append(self, recipe):
        self.recipes.append(recipe)

    def get(self, recipeId):
        result = filter(lambda element: element.id == recipeId, self.recipes)
        result = list(result)
        if len(result) > 0:
            result = result[0]
        else:
            result = None
        return result

    def delete(self, recipeId):
        self.recipes = list(filter(lambda element: element.id != recipeId, self.recipes))

    def __len__(self):
        return len(self.recipes)

    def __iter__(self):
        return self.recipes.__iter__()


users = dict()
recipes = RecipeCollection()


@App.path(model=Person, path='/api/v1/person/{personId}')
def getPerson(personId):
    personId = int(personId)
    print('Retrieving record: {}'.format(personId))

    result = users.get(personId)
    print(result)
    return result


@App.view(model=Person)
def getPerson(self, _):
    """Formats the data to be delivered to the client.

    :param self: The Person object which was retrieved by getPerson above
    """

    result = json.dumps(dict(firstname=self.firstname, lastname=self.lastname))
    print("Result: {}".format(result))
    return result


# Recipe collection: /api/v1/recipes[?reload]
@App.path(model=RecipeCollection, path='/api/v1/recipes')
def getRecipes(extra_parameters):

    global recipes
    if 'reload' in extra_parameters:
        recipes.reload()

    return recipes


@App.json(model=RecipeCollection)
def renderRecipeCollectionJson(recipeCollection, _):
    result = [r.toJson() for r in recipeCollection]
    print(result)
    return result


@App.json(model=RecipeCollection, request_method='POST')
def addRecipe(recipeCollection, request):
    payload = json.loads(request.body)
    payload['id'] = "R{}".format(len(recipes) + 1)
    recipe = Recipe(payload)
    recipes.append(recipe)


# Single Recipe
@App.path(model=Recipe, path='/api/v1/recipe/{recipeId}')
def getRecipe(self, recipeId):
    result = recipes.get(recipeId)
    return result


@App.json(model=Recipe)
def renderRecipeJson(recipe, _):
    return recipe.toJson()


@App.json(model=Recipe, request_method='DELETE')
def deleteRecipe(self, _):
    recipes.delete(self.id)


def main():
    global users, recipes

    users[1] = Person('Peter', 'Shaw')
    users[2] = Person('Justus', 'Jonas')

    recipes.reload()

    morepath.run(App())


if __name__ == "__main__":
    main()
