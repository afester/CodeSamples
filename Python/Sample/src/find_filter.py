import sys


def sqrt(value: float) -> float:

    step = 1

    prev_result = value
    lower = 0
    upper = prev_result
    result = 0
    while abs(result - prev_result) > sys.float_info.epsilon:
        prev_result = result

        result = (lower + upper) / 2
        # print(f"   {value} {step} {upper-lower} {sys.float_info.epsilon} {prev_result} {result}")
        # print(f"   {value} {step} {upper} {lower} {prev_result} {result}")
        if result ** 2 > value:
            upper = result
        else:
            lower = result

        step += 1
    # print(f"{step} iterations.")
    return result

def main(argv):
    print(f"sqrt(2)={sqrt(2)}")
    # print(f"sqrt(3)={sqrt(3)}")
    # print(f"sqrt(5)={sqrt(5)}")
    # print(f"sqrt(6)={sqrt(6)}")
    # print(f"sqrt(7)={sqrt(7)}")
    # print(f"sqrt(8)={sqrt(8)}")
    # print(f"sqrt(15)={sqrt(15)}")
    # print(f"sqrt(20)={sqrt(20)}")
    # print(f"sqrt(25)={sqrt(25)}")
    # print(f"sqrt(30)={sqrt(30)}")
    # print(f"sqrt(35)={sqrt(35)}")
    # print(f"sqrt(50)={sqrt(50)}")
    # print(f"sqrt(100)={sqrt(100)}")
    # print(f"sqrt(150)={sqrt(150)}")
    # print(f"sqrt(200)={sqrt(200)}")
    # print(f"sqrt(250)={sqrt(250)}")
    # print(f"sqrt(256)={sqrt(256)}")

if __name__ == "__main__":
    main(sys.argv)
