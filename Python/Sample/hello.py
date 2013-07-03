# Import the "random" module:
import random

# Python 3: "The print statement has been replaced with a print() function"
# http://docs.python.org/3.0/whatsnew/3.0.html
print('Hello World')

def fac(x):
    if x < 2:
        return 1;
    else:
        return x * fac(x-1);

print(fac(100))
# 93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000
# In python, the size of int's are only limited by available memory 


helloStr = 'Hello World'
print(helloStr[4])

# String is immutable
# helloStr[2] = 'p'

# From int to String:
myInt = fac(10)
print(myInt)
myStr = str(myInt)
print(myStr, len(myStr))

# From String to int:
myStr = "123"
myInt = int(myStr)
print(type(myInt))

myFloat = 3.4
print(type(myFloat))
print(type(myStr))

print('myStr is myInt: ' + str((myStr is myInt)))
print('myStr == myInt: ' + str((myStr == myInt)))
print('myStr == str(myInt): ' + str((myStr == str(myInt))))

# Tuple
myTuple = 3, 5, 7   # Valid syntax!
print(myTuple)
myTuple = (2, 4, 6) # Syntax usually used
print(myTuple, len(myTuple), type(myTuple))
print(myTuple[2])
# Tuples are immutable:
# myTuple[2] = 10

# List
myList = [3, 5, 7]
print(myList, len(myList), type(myList))
print(myList[2])
# Lists are mutable:
myList[2] = 10
print(myList)

# Comparison operators
x1 = 5
x2 = 7
print("x1 == x2: " + str(x1 == x2))
print("x1 < x2: " + str(x1 < x2))
print("x1 > x2: " + str(x1 > x2))
print("x1 != x2: " + str(x1 != x2))
print("2 < x1 < 6: " + str(2 < x1 < 6))
print("8 < x1 < 16: " + str(8 < x1 < 16))
# print("x1 <> x2: " + str(x1 <> x2)) # Python 3 has removed the <> operator

# Membership operator
# "For lists and tuples, the in operator uses a linear search which can be slow
# for very large collections
print("llo in %s" % helloStr + ": " + str("llo" in helloStr)) 
print("loo in %s" % helloStr + ": " + str("loo" in helloStr))
print("4 in myList: " + str(4 in myList) )
print("4 not in myList: " + str(4 not in myList) )
print("10 in myList: " + str(10 in myList) )

# Logical operators
# "and" and "or" use short-circuit logic and return the operand that determined the result
a = 5
b = 2
c = 0
print(a and b)  # 2
print(b and c)  # 0
print(c and a)  # 0


# "In Python, a Boolean expression evaluates to False if it is the predefined
# constant False, the special object None, an empty sequence or collection
# (e.g., an empty string, list, or tuple), or a numeric data item of value 0;
# anything else is considered to be True."
if True:
    print("True")
if not False:
    print("not False")
if not 0:
    print("not 0")
if not None:
    print("not None")
if not []:
    print("not []")
if [2,3]:
    print("[2,3]")
if not ():
    print("not ()")
if (4,5):
    print("(4,5)")
if not {}:
    print("not {}")
if {5, 6}:
    print("{5, 6}")

# What is called "block" in other languages is called "suite" in Python.
# Some statements require that a suite is present - sometimes this can be
# an empty one for whatever reason. In languages like Java or C/C++, an empty
# block can be implemented like {}. Python provides the statement "pass" in order
# to implement an empty suite - this is basically a NOP.
x = 10.4 
if x > 100:
    print("> 100")
elif x > 50:
    pass
else:
    print("<= 50")

while x > 9.0:
    x = x - 0.1
    print(x) 

for e in myList:
    print(e)

for c in "Hello":
    print(c)

try:
    print("Trying ...")
    raise 1
except: # Exceptions need to be derived from BaseException - something like "except int as i:" is not allowed
    print("Catched")
finally:
    print("Finally")

############# OPERATORS
    
a = 10
b = 3
print(a / b)        # produces floating point value - 3.333333333333333333333
print(int(a / b))   # 3
print(a // b)       # 3

c = a / b
print(str(c) + " " + str(type(c)))

b = 5
c = b
b += 5
print("%s %s" % (b, c) )

# Lists are mutable
l = [3, 4, 5]
l1 = l
print(l)
l += [6]
print(str(l) + " l is l1:" + str(l is l1)) # True

# Tuples are immutable
t = (2, 3, 4)
t1 = t
print(t)
t += (5,)
print(str(t) + " t is t1:" + str(t is t1)) # False

name = input("Name: ")
print("Hello %s" % name)

def playSimpleGame():
    random.seed()
    value = random.randint(0, 1000)
    print("I have created a number between 0 and 1000 (both inclusive)")

    count = 0
    found = False
    while not found:
        # returns a string with no trailing end of line characters. Simply
        # pressing "enter" returns an empty string.
        # Throws EOFError if an end of file is recognized.
        try: 
            guess = input("Please enter your guess: ")
            guessedValue = int(guess)
        except:
            guessedValue = None  # ignore all errors - assume no valid input

        # if user entered a valid number:
        if guessedValue is not None:
            count += 1
            if guessedValue < value:
                print("Your guess is too small")
            elif guessedValue > value:
                print("Your guess is too large")
            else:
                print("Hey, you got it in %s tries!" % count)
                found = True

playSimpleGame()


if __name__ == '__main__':
    pass
