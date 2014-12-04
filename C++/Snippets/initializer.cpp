
#include <iostream>

#include "initializer.h"
#include "Samples.h"

using namespace std;


TestInitialize::TestInitialize() : value(42) {
	cerr << "Default constructor" << endl;
}


TestInitialize::TestInitialize(int val) : value(val) {
	cerr << "Constructor(int)" << endl;
}


int TestInitialize::getValue() {
	return value;
}

// http://stackoverflow.com/questions/620137/do-the-parentheses-after-the-type-name-make-a-difference-with-new/620402#620402
void initializerSample() {
	cerr << "Testing initializers ...\n=================" << endl;

	// http://stackoverflow.com/questions/620137/do-the-parentheses-after-the-type-name-make-a-difference-with-new/620402#620402

	// C++98:
	// * Zero-initialization
	// * Default-initialization

	// C++03:
	// * Value initialization, in addition

	int* i1 = new int;
	cerr << "int* i1 = new int; => *i1 = " << *i1 << endl;

	int* i2 = new int();	// zero initialization
	cerr << "int* i2 = new int(); => *i2 = " << *i2 << endl;

	// C++11: Uniform initialization syntax
	int i3;
	cerr << "int i3; => i3 = " << i3 << endl;	// i3 is uninitialized!

	//int i4();	// NOT the same as "new int()" !!
				// declares a function named i4 returning an int!!
	int i4{};	// C++11 only
	cerr << "int i4; => i4 = " << i4 << endl;








	// TestInitialize x();		// WRONG!!!

	TestInitialize x;			// CORRECT!!
	cerr << "Value: " << x.getValue() << endl;

	TestInitialize y(3);		// CORRECT!!
	cerr << "Value: " << y.getValue() << endl;
}



InitializerSample::InitializerSample() : Sample("Initializer sample") {
}

void InitializerSample::run() {
    initializerSample();
}
