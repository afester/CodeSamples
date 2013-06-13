
// http://www.codeproject.com/Articles/3515/How-To-Organize-Template-Source-Code

#include "Array2D.cpp"	// need to see the template definitions here!

// explicitly instantiate all templates - drawback: all methods are
// instantiated, lazy instantiation is not possible!
template class Array2D<int>;
