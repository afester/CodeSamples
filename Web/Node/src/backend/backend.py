import json
import sys

import morepath
from database import Person, Recipe, RecipeCollection, Database


class App(morepath.App):
    pass


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
    result = RecipeCollection.query()
    return result


@App.json(model=RecipeCollection)
def renderRecipeCollectionJson(recipeCollection, _):
    result = [r.toJson() for r in recipeCollection]
    return result


@App.json(model=RecipeCollection, request_method='POST')
def addRecipe(recipeCollection, request):
    payload = json.loads(request.body)
    payload['id'] = None # "R{}".format(len(recipes) + 1)
    recipe = Recipe(payload)
    RecipeCollection.append(recipe)


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
    Recipe.delete(self.id)


def testDatabase():
    print("Checking database ...")
    recipes = RecipeCollection.query()
    for rec in recipes:
        print(" R=>: {}".format(rec))


def createDB():
    print("Creating database ...")
    db = Database()
    db.createSchema()


def dropDB():
    print("Dropping database schema ...")
    db = Database()
    db.dropSchema()


def importDB():
    print("Importing data ...")
    RecipeCollection.importRecipes()


def main(argv):
    if '--testDB' in argv:
        testDatabase()
    elif '--createDB' in argv:
        dropDB()
    elif '--dropDB' in argv:
        dropDB()
    elif '--importDB' in argv:
        importDB()
    else:
        morepath.run(App())


if __name__ == "__main__":
    main(sys.argv)
