/**
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

    void operator()() {
       std::cerr << "void operator()()" << std::endl;
    }

    void operator()(int val) {
       std::cerr << "void operator()(int)" << std::endl;
    }
};



int main() {
    A a;
    a();
    a(42);

    return 0;
}
