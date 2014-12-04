#include <iostream>
#include "Samples.h"


SizeofSample::SizeofSample() : Sample("Sizeof sample") {
}

void SizeofSample::run() {
	const char* p = "Hello";

	std::cerr << "sizeof(int): " << sizeof(int) << std::endl;
	std::cerr << "sizeof(\"Hello World\"): " << sizeof("Hello World") << std::endl;
	std::cerr << "sizeof(p): " << sizeof(p)  << std::endl;
}
