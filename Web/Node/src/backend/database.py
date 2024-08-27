import json
import pyodbc


class Database(object):

    def __init__(self):
        self.connection = None

    def createSchema(self):
        print("Building schema ...")
        cursor = self.connection.cursor()

        cursor.execute("CREATE TABLE recipes ("
                       "  id varchar(10) PRIMARY KEY,"
                       "  title varchar(255),"
                       "  rating int,"
                       "  instructions varchar(255));")

        cursor.execute("CREATE TABLE ingredients ("
                       "  recipeId varchar(10),"
                       "  posNo int,"
                       "  amount int,"
                       "  unit varchar(20),"
                       "  ingredientName varchar(100)"
                       "  PRIMARY KEY(recipeId, posNo));")
        self.connection.commit()
        cursor.close()

    def dropSchema(self):
        if not self.connection:
            self.connect()

        print("Dropping schema ...")
        cursor = self.connection.cursor()
        cursor.execute("DROP TABLE recipes")
        cursor.execute("DROP TABLE ingredients")
        self.connection.commit()
        cursor.close()

    def connect(self):
        with open("database.json", "r") as reader:
            dbParams = json.load(reader)

        print("Connecting to {} ...".format(dbParams))
        self.connection = pyodbc.connect("DRIVER={driver_name};"
                                         "SERVER={server_name};"
                                         "DATABASE={database_name};"
                                         "UID={user_id_of_database};"
                                         "PWD={password_of_database};".format(
            driver_name=dbParams["driver"], server_name=dbParams["hostname"],
            database_name=dbParams["database"], user_id_of_database=dbParams["username"],
            password_of_database=dbParams["password"]))
        print("Connected.")

    def query(self, stmt):
        if not self.connection:
            self.connect()

        cursor = self.connection.cursor()
        cursor.execute(stmt)
        result = []
        for x in cursor:
            result.append(x)
        return result

    def insert(self, stmt):
        if not self.connection:
            self.connect()

        cursor = self.connection.cursor()
        cursor.execute(stmt)
        self.connection.commit()
        cursor.close()

    def delete(self, stmt):
        if not self.connection:
            self.connect()


theDatabase = Database()


class Person(object):
    def __init__(self, firstname, lastname):
        self.firstname = firstname
        self.lastname = lastname

    def __str__(self):
        return "Person[firstname={}, lastname={}]".format(self.firstname, self.lastname)


class Recipe(object):
    def __init__(self, recId, title=None, rating=None):
        if type(id) == dict:
            jsonData = recId
            # print(jsonData)
            self.id = jsonData['id']
            self.title = jsonData['title']
            self.rating = int(jsonData['rating'])
            self.ingredients = jsonData['ingredients']
            self.instructions = jsonData['instructions']
            # print("=====> {}".format(self.ingredients))
        else:
            self.id = recId
            self.title = title
            self.rating = rating
            self.ingredients = list()
            self.instructions = list()

    @staticmethod
    def delete(recipeId):
        pass
        # self.recipes = list(filter(lambda element: element.id != recipeId, self.recipes))

    def toJson(self):
        return dict(id=self.id,
                    title=self.title,
                    rating=self.rating,
                    ingredients=self.ingredients,
                    instructions=self.instructions)

    def insertIntoDB(self):
        print(" INSERT: {}".format(self))
        theDatabase.insert("""INSERT INTO recipes(id, title, rating, instructions) 
                              VALUES('{}', '{}', {}, '{}')""".format(self.id, self.title, self.rating,
                                                                     self.instructions))
        posNo = 1
        for ingredient in self.ingredients:
            theDatabase.insert("""INSERT INTO ingredients(recipeId, posNo, amount, unit, ingredientName)
                                  VALUES('{}', {}, {}, '{}', '{}')""".format(self.id, posNo,
                                                                             ingredient["amount"],
                                                                             ingredient["unit"],
                                                                             ingredient["ingredient"]))
            posNo += 1

    def __str__(self):
        return "Recipe[id={}, title={}]".format(self.id, self.title)

    def __repr__(self):
        return "Recipe[id={}, title={}]".format(self.id, self.title)


class RecipeCollection(object):
    def __init__(self):
        self.recipes = list()

    @staticmethod
    def query():
        result = RecipeCollection()
        result.reload()
        return result

    @staticmethod
    def append(recipe):
        print("ADD: {}".format(recipe))
        # self.recipes.append(recipe)

    def reload(self):
        recipes = dict()
        rows = theDatabase.query("""SELECT * 
                                    FROM recipes rec
                                    JOIN ingredients ing on (ing.recipeId = rec.id)
                                    ORDER BY rec.id, ing.posNo""")
        for row in rows:
            recId = row[0]
            recipe = recipes.get(recId)
            if not recipe:
                recipe = Recipe(row[0], row[1], row[2])
                recipes[row[0]] = recipe
            recipe.ingredients.append(dict(amount=row[6], unit=row[7], ingredient=row[8]))

        self.recipes = [x for x in recipes.values()]

    def get(self, recipeId):
        result = filter(lambda element: element.id == recipeId, self.recipes)
        result = list(result)
        if len(result) > 0:
            result = result[0]
        else:
            result = None
        return result

    @staticmethod
    def importRecipes():
        with open("recipes.json", "r") as reader:
            recipesData = json.load(reader)
            for recipeData in recipesData:
                recipe = Recipe(recipeData)
                recipe.insertIntoDB()

    def exportRecipes(self):
        pass

    def __str__(self):
        return "{} Objects".format(len(self))

    def __len__(self):
        return len(self.recipes)

    def __iter__(self):
        return self.recipes.__iter__()


users = dict()
# recipes = RecipeCollection()
