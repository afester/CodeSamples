import sys
from functools import reduce

def main(argv):
    animals = ['snake', 'camel', 'etruscan shrew', 'ant', 'hippopotamus', 'giraffe', 'ape']

    filtered = filter(lambda x: x.startswith('a'), animals)
    print(list(filtered))

    longest = reduce(lambda a, b: a if len(a) > len(b) else b, animals)
    print(longest)

    str_and_length = map(lambda e: {"text": e, "length": len(e)}, animals)
    print(list(str_and_length))

    found = next(filter(lambda x: x == "ant", animals), None)
    print(found)

    #
    # def ff(e):
    #     print(f"ff({e})")
    #     return e == "ant"
    #
    # filtered = filter(ff, animals)
    # print(f"RESULT: {list(filtered)}")
    #
    # found = next(filter(ff, animals), None)
    # print(f"RESULT: {found}")

if __name__ == "__main__":
    main(sys.argv)
