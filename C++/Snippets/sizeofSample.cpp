#include <iostream>

void sizeofSample()  {
	const char* p = "Hello";

	std::cerr << "sizeof(int): " << sizeof(int) << std::endl;
	std::cerr << "sizeof(\"Hello World\"): " << sizeof("Hello World") << std::endl;
	std::cerr << "sizeof(p): " << sizeof(p)  << std::endl;
}
