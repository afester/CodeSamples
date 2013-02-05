/**
 * Rule 2: "If a new object is being defined (such as a3 in the sample below),
 *          a constructor has to be called; it can not be an assignment.
 *          If no new object is being defined (such as in the a4 = a3 statement
 *          below), no constructor can be involved, so it is an assignment."
 */

#include <iostream>

class A {
    int m;

public:
    explicit A(int val = 0) {
       std::cerr << "explicit A(int)" << std::endl;

       m = val * 10;
    }

    A(const A& other) {
       std::cerr << "A(const A&)" << std::endl;
    }

    A& operator=(const A& other) {
       std::cerr << "A& operator=(const A&)" << std::endl;
    }
};



int main() {
    A a1(42);	// constructor
    A a2(a1);   // copy constructor

    A a3 = a2;  // copy constructor (!!)

    A a4;	// constructor
    a4 = a3;	// assignment operator (!!)

    return 0;
}
