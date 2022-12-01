import sys


def generatePlanets():
    for x in  ['Merkur', 'Venus', 'Erde', 'Mars', 'Jupiter', 'Saturn', 'Uranus', 'Neptun']:
        yield x


# A function which uses a yield statement is a generator
def simple_coroutine():
    print("Running coroutine ...")
    x = yield
    print("Got: {}".format(x))
    y = yield
    print("Got: {}".format(y))


def main(args):
    # for p in generatePlanets():
    #     print(p)
    planets = generatePlanets()     # Calling a generator function returns a generator object! (<class 'generator'>)
    for p in planets:
        print(p)

    print("Calling ...")
    coro = simple_coroutine()
    next(coro)                  # Priming the coroutine - after this call the coroutine is suspended at the first yield
    coro.send("Hello")
    coro.send("World")
    print("Called")


if __name__ == '__main__':
    main(sys.argv)
