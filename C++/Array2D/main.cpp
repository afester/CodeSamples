#include <stdio.h>
#include <iostream>

#include "Array2D.h"

int main() {
//	 Row<int> x;     // Disallow construction
//	 Row<int> x(0);  // Disallow construction

    Array2D<int> array(10, 10);

    array.at(2, 2) = 42;
    std::cerr << array.at(2, 2) << std::endl;
    std::cerr << array[2][2] << std::endl;

    // check uninitialized elements
    std::cerr << array.at(1, 1) << std::endl;

    // print the array
    std::cerr << array.toString() << std::endl;

    // check array out of bounds
    std::cerr << array.at(8, 10) << std::endl;
    std::cerr << array[9][11] << std::endl;

    // check array copy construction - not currently allowed!
//    {
//    	Array2D<int> array2(array);	// Disallowed - copy constructor is private
//    }
//    std::cerr << array[2][2] << std::endl;

    // check array copy assignment - not currently allowed!
//    {
//    	Array2D<int> array2(0,0);
//    	array2 = array;				// Disallowed - assignment constructor is private
//    }
//    std::cerr << array[2][2] << std::endl;

    return 0;
}
