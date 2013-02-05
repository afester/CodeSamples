/**
 * Rule 1: "Unless you have a good reason for allowing a constructor to
 *          be used for implicit type conversions, declare it explicit."
 */

class A {
    int m;

public:
    explicit A(int val = 0) {
       m = val * 10;
    }
};


void takeObject(const A& obj) {
}

int main() {
    A a(42);
    takeObject(a);

//    takeObject(43); // Constructor declared explicit,
                      // can not perform implicit type conversion!
    takeObject(A(43));

    return 0;
}
